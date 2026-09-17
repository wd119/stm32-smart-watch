#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
把 docs/需求分析与可行性研究.md 转成同名的 .docx。

只支持本工程实际用到的那一小撮 Markdown 语法，不做通用转换器：
    标题      # ## ### ####
    段落      普通文本行
    粗体      **文字**
    行内代码  `代码`
    无序列表  - 项目
    有序列表  1. 项目
    表格      | a | b |  （第二行是 |---|---| 分隔行时按表格处理）
    代码块    ``` 围栏
    分页      <!-- pagebreak -->
    占位符    <!-- 待填 -->  ->  （待填）

版面约定：源文件里第一个 <!-- pagebreak --> 之前的内容当作封面，
在该处分页并自动插入目录页。即 封面 → 目录 → 正文。

用法：
    python docs/build_docx.py
    python docs/build_docx.py 别的文件.md 输出.docx
"""

import re
import sys
from pathlib import Path

from docx import Document
from docx.enum.table import WD_TABLE_ALIGNMENT
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Pt, RGBColor

# ---------------------------------------------------------------- 字体

ASCII_FONT = "Times New Roman"
CJK_FONT = "宋体"
CJK_HEAD_FONT = "黑体"
MONO_FONT = "Consolas"


def style_run(run, ascii_font=ASCII_FONT, cjk_font=CJK_FONT,
              size=None, bold=None, mono=False):
    """设定一个 run 的字体，中英文分别指定。"""
    if mono:
        ascii_font, cjk_font = MONO_FONT, MONO_FONT
    if size is not None:
        run.font.size = Pt(size)
    if bold is not None:
        run.font.bold = bold
    run.font.name = ascii_font
    rpr = run._element.get_or_add_rPr()
    rfonts = rpr.find(qn("w:rFonts"))
    if rfonts is None:
        rfonts = OxmlElement("w:rFonts")
        rpr.append(rfonts)
    rfonts.set(qn("w:ascii"), ascii_font)
    rfonts.set(qn("w:hAnsi"), ascii_font)
    rfonts.set(qn("w:eastAsia"), cjk_font)


def shade(paragraph, hex_fill="F2F2F2"):
    """给段落加底色。"""
    pr = paragraph._p.get_or_add_pPr()
    shd = OxmlElement("w:shd")
    shd.set(qn("w:val"), "clear")
    shd.set(qn("w:color"), "auto")
    shd.set(qn("w:fill"), hex_fill)
    pr.append(shd)


# ---------------------------------------------------------------- 域代码

def _field(paragraph, instruction, placeholder=""):
    """插入 Word 域（用于目录、页码）。打开文档后需按 F9 更新。"""
    run = paragraph.add_run()
    begin = OxmlElement("w:fldChar")
    begin.set(qn("w:fldCharType"), "begin")
    instr = OxmlElement("w:instrText")
    instr.set(qn("xml:space"), "preserve")
    instr.text = instruction
    sep = OxmlElement("w:fldChar")
    sep.set(qn("w:fldCharType"), "separate")
    text = OxmlElement("w:t")
    text.text = placeholder
    end = OxmlElement("w:fldChar")
    end.set(qn("w:fldCharType"), "end")
    for node in (begin, instr, sep, text, end):
        run._r.append(node)
    return run


def add_header(section, text):
    header = section.header
    p = header.paragraphs[0] if header.paragraphs else header.add_paragraph()
    p.text = ""
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = p.add_run(text)
    style_run(run, size=9)
    run.font.color.rgb = RGBColor(0x60, 0x60, 0x60)


def add_page_number_footer(section, text_left):
    """页脚：左边固定文字，右边自动页码。"""
    footer = section.footer
    p = footer.paragraphs[0] if footer.paragraphs else footer.add_paragraph()
    p.text = ""
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = p.add_run(text_left + "    第 ")
    style_run(run, size=9)
    _field(p, "PAGE", "1")
    run = p.add_run(" 页")
    style_run(run, size=9)
    for r in p.runs:
        style_run(r, size=9)


def add_toc(doc):
    """插入目录页。目录域需在 Word 中按 F9 更新才会显示条目。"""
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    p.paragraph_format.space_after = Pt(18)
    run = p.add_run("目  录")
    style_run(run, size=16, bold=True, cjk_font=CJK_HEAD_FONT)
    field_p = doc.add_paragraph()
    _field(field_p, r'TOC \o "1-3" \h \z \u',
           "（在 Word 中按 Ctrl+A 再按 F9 生成目录）")


# ---------------------------------------------------------------- 解析

PAGEBREAK_RE = re.compile(r"^<!--\s*pagebreak\s*-->$")
PENDING_RE = re.compile(r"<!--\s*待填\s*-->")
COMMENT_RE = re.compile(r"<!--.*?-->")
BULLET_RE = re.compile(r"^[-*]\s+(.*)$")
NUMBER_RE = re.compile(r"^(\d+)\.\s+(.*)$")
HEADING_RE = re.compile(r"^(#{1,6})\s+(.*)$")
RULE_RE = re.compile(r"^\|[\s:\-|]+\|$")


def clean(text):
    """去掉 HTML 注释，占位注释换成可读文字。"""
    text = PENDING_RE.sub("（待填）", text)
    return COMMENT_RE.sub("", text).strip()


def add_runs(paragraph, text, size=10.5, base_bold=False):
    """把含 **粗体** 与 `代码` 的文本写进段落。"""
    text = clean(text)
    for piece in re.split(r"(\*\*.+?\*\*|`[^`]+`)", text):
        if not piece:
            continue
        if piece.startswith("**") and piece.endswith("**"):
            run = paragraph.add_run(piece[2:-2])
            style_run(run, size=size, bold=True)
        elif piece.startswith("`") and piece.endswith("`"):
            run = paragraph.add_run(piece[1:-1])
            style_run(run, size=size - 0.5, mono=True)
            run.font.color.rgb = RGBColor(0xA0, 0x30, 0x30)
        else:
            run = paragraph.add_run(piece)
            style_run(run, size=size, bold=base_bold)


def split_row(line):
    """拆一行表格，去掉首尾空列。"""
    return [clean(c) for c in line.strip().strip("|").split("|")]


def convert(md_path: Path, docx_path: Path):
    lines = md_path.read_text(encoding="utf-8").splitlines()

    doc = Document()

    normal = doc.styles["Normal"]
    normal.font.name = ASCII_FONT
    normal.font.size = Pt(10.5)
    normal.element.rPr.rFonts.set(qn("w:eastAsia"), CJK_FONT)

    sec = doc.sections[0]
    add_header(sec, "基于 STM32F103 的可编程手表    需求分析与可行性研究")
    add_page_number_footer(sec, "可编程手表")

    i = 0
    first_heading_done = False
    toc_done = False

    while i < len(lines):
        line = lines[i]
        stripped = line.strip()

        # --- 分页（第一个分页符 = 封面结束，在此插入目录页）---
        if PAGEBREAK_RE.match(stripped):
            doc.add_page_break()
            if not toc_done:
                toc_done = True
                add_toc(doc)
                doc.add_page_break()
            i += 1
            continue

        # --- 空行 ---
        if not stripped:
            i += 1
            continue

        # --- 代码块 ---
        if stripped.startswith("```"):
            i += 1
            buf = []
            while i < len(lines) and not lines[i].strip().startswith("```"):
                buf.append(lines[i])
                i += 1
            i += 1  # 跳过收尾围栏
            p = doc.add_paragraph()
            p.paragraph_format.space_before = Pt(4)
            p.paragraph_format.space_after = Pt(4)
            p.paragraph_format.left_indent = Pt(12)
            shade(p)
            run = p.add_run("\n".join(buf))
            style_run(run, size=8.5, mono=True)
            continue

        # --- 标题 ---
        m = HEADING_RE.match(stripped)
        if m:
            level, text = len(m.group(1)), clean(m.group(2))
            if not first_heading_done:
                p = doc.add_paragraph()
                p.alignment = WD_ALIGN_PARAGRAPH.CENTER
                p.paragraph_format.space_before = Pt(72)
                p.paragraph_format.space_after = Pt(36)
                run = p.add_run(text)
                style_run(run, size=24, bold=True, cjk_font=CJK_HEAD_FONT)
                first_heading_done = True
            else:
                h = doc.add_heading(level=min(level, 4))
                h.text = ""
                run = h.add_run(text)
                size = {1: 18, 2: 15, 3: 12.5, 4: 11.5}.get(level, 11)
                style_run(run, size=size, bold=True, cjk_font=CJK_HEAD_FONT)
                run.font.color.rgb = RGBColor(0, 0, 0)
            i += 1
            continue

        # --- 表格 ---
        if stripped.startswith("|"):
            rows = []
            while i < len(lines) and lines[i].strip().startswith("|"):
                rows.append(lines[i])
                i += 1
            header = split_row(rows[0])
            body = [split_row(r) for r in rows[1:] if not RULE_RE.match(r.strip())]
            body = [r for r in body if any(r)]
            if not body:
                continue
            ncol = len(header)
            table = doc.add_table(rows=1, cols=ncol)
            table.style = "Table Grid"
            table.alignment = WD_TABLE_ALIGNMENT.CENTER
            for j, cell_text in enumerate(header[:ncol]):
                cell = table.rows[0].cells[j]
                cell.text = ""
                add_runs(cell.paragraphs[0], cell_text, size=9.5, base_bold=True)
            for row in body:
                cells = table.add_row().cells
                for j in range(ncol):
                    val = row[j] if j < len(row) else ""
                    cells[j].text = ""
                    add_runs(cells[j].paragraphs[0], val, size=9.5)
            doc.add_paragraph()
            continue

        # --- 无序列表 ---
        m = BULLET_RE.match(stripped)
        if m:
            p = doc.add_paragraph(style="List Bullet")
            add_runs(p, m.group(1))
            i += 1
            continue

        # --- 有序列表 ---
        m = NUMBER_RE.match(stripped)
        if m:
            p = doc.add_paragraph(style="List Number")
            add_runs(p, m.group(2))
            i += 1
            continue

        # --- 普通段落 ---
        p = doc.add_paragraph()
        p.paragraph_format.first_line_indent = Pt(21)
        p.paragraph_format.space_after = Pt(6)
        add_runs(p, stripped)
        i += 1

    doc.save(str(docx_path))
    return docx_path


def main():
    here = Path(__file__).resolve().parent
    md = Path(sys.argv[1]) if len(sys.argv) > 1 else here / "需求分析与可行性研究.md"
    out = Path(sys.argv[2]) if len(sys.argv) > 2 else md.with_suffix(".docx")
    convert(md, out)
    print(f"已生成：{out}")
    print(f"大小  ：{out.stat().st_size:,} 字节")


if __name__ == "__main__":
    main()
