#!/usr/bin/env python3

import clang.cindex
import os
import sys
import time
import re
import argparse
from typing import List, Set, Tuple, Optional, Dict

# --- Configuration for libclang ---
# On Windows, ensure LLVM/bin is in your PATH, or set it here:
# clang.cindex.Config.set_library_path('C:/Program Files/LLVM/bin') 

class CElement:
    def __init__(self, 
                 file_name: str, 
                 name: str,
                 element_type: str, 
                 line: int,
                 display_string: str, 
                 raw_first_line: str, 
                 return_type: Optional[str] = None 
                ):
        self.file_name: str = file_name
        self.name: str = name
        self.element_type: str = element_type
        self.line: int = line
        self.display_string: str = display_string
        self.raw_first_line: str = raw_first_line 
        self.return_type: Optional[str] = return_type

    def __repr__(self):
        return (f"CElement(file='{self.file_name}', name='{self.name}', "
                f"type='{self.element_type}', line={self.line}, display='{self.display_string[:40]}...')")

def get_source_text(extent: clang.cindex.SourceRange) -> str:
    if not extent or not extent.start.file or not extent.start.file.name:
        return ""
    try:
        file_path = os.path.abspath(extent.start.file.name)
        with open(file_path, 'rb') as f_bytes:
            f_bytes.seek(extent.start.offset)
            source_bytes = f_bytes.read(extent.end.offset - extent.start.offset)
            return source_bytes.decode('utf-8', errors='replace')
    except Exception:
        return "" 

def extract_elements_from_file(
    file_path: str, 
    common_include_paths: List[str]
) -> List[CElement]:
    
    index = clang.cindex.Index.create()
    args = ['-Xclang', '-detailed-preprocessing-record']
    for p in common_include_paths: args.append(f'-I{p}')
    args.append(f'-I{os.path.dirname(os.path.abspath(file_path))}')

    try:
        tu = index.parse(os.path.abspath(file_path), args=args)
    except clang.cindex.LibclangError: return []
    if not tu: return []

    elements: List[CElement] = []
    handled_anon_compound_locations: Set[Tuple[str, int]] = set()
    processed_macro_names: Set[str] = set() 

    abs_file_path_for_comparison = os.path.abspath(file_path)
    base_file_name = os.path.basename(file_path)

    for cursor in tu.cursor.get_children():
        if not cursor.location.file or \
           not cursor.location.file.name or \
           os.path.abspath(cursor.location.file.name) != abs_file_path_for_comparison:
            continue
        
        if (abs_file_path_for_comparison, cursor.location.line) in handled_anon_compound_locations:
            continue

        el_name = cursor.spelling or ""
        el_line = cursor.extent.start.line
        
        raw_full_definition_text = get_source_text(cursor.extent)
        raw_first_line = raw_full_definition_text.splitlines()[0].strip() if raw_full_definition_text else ""

        if el_line == 0 or not raw_first_line: continue

        el_type_str = ""
        display_str = raw_first_line 
        el_return_type: Optional[str] = None

        if cursor.kind == clang.cindex.CursorKind.TYPEDEF_DECL:
            underlying_type = cursor.underlying_typedef_type
            underlying_type_decl = underlying_type.get_declaration()
            
            is_anon_compound_typedef = False
            if underlying_type_decl.location.file and \
               os.path.abspath(underlying_type_decl.location.file.name) == abs_file_path_for_comparison and \
               not underlying_type_decl.spelling: 

                compound_kind_str = ""
                members = []
                if underlying_type_decl.kind == clang.cindex.CursorKind.ENUM_DECL:
                    compound_kind_str = "enum"
                    el_type_str = "Typedef Enum"
                    for child in underlying_type_decl.get_children():
                        if child.kind == clang.cindex.CursorKind.ENUM_CONSTANT_DECL:
                            members.append(f"{child.spelling}={child.enum_value}")
                    member_display = ", ".join(members)
                    is_anon_compound_typedef = True
                elif underlying_type_decl.kind in (clang.cindex.CursorKind.STRUCT_DECL, clang.cindex.CursorKind.UNION_DECL):
                    compound_kind_str = "struct" if underlying_type_decl.kind == clang.cindex.CursorKind.STRUCT_DECL else "union"
                    el_type_str = f"Typedef {compound_kind_str.capitalize()}"
                    for child in underlying_type_decl.get_children():
                        if child.kind == clang.cindex.CursorKind.FIELD_DECL:
                            members.append(f"{child.type.spelling} {child.spelling}")
                    member_display = "; ".join(members) + (";" if members else "")
                    is_anon_compound_typedef = True

                if is_anon_compound_typedef:
                    display_str = f"typedef {compound_kind_str} {{ {member_display} }} {el_name};"
                    handled_anon_compound_locations.add(
                        (abs_file_path_for_comparison, underlying_type_decl.location.line)
                    )
            
            if not is_anon_compound_typedef: 
                el_type_str = "Typedef"
        
        elif cursor.kind == clang.cindex.CursorKind.ENUM_DECL:
            if cursor.is_definition():
                el_type_str = "Enum"
                members = [f"{c.spelling}={c.enum_value}" for c in cursor.get_children() if c.kind == clang.cindex.CursorKind.ENUM_CONSTANT_DECL]
                member_display = ", ".join(members)
                tag = el_name if el_name else ""
                display_str = f"enum {tag} {{ {member_display} }};".replace("enum  {", "enum {")
                if not el_name: el_name = f"AnonymousEnum_L{el_line}"

        elif cursor.kind in (clang.cindex.CursorKind.STRUCT_DECL, clang.cindex.CursorKind.UNION_DECL):
            if cursor.is_definition():
                el_type_str = "Struct" if cursor.kind == clang.cindex.CursorKind.STRUCT_DECL else "Union"
                members = [f"{c.type.spelling} {c.spelling}" for c in cursor.get_children() if c.kind == clang.cindex.CursorKind.FIELD_DECL]
                member_display = "; ".join(members) + (";" if members else "")
                tag = el_name if el_name else ""
                display_str = f"{el_type_str.lower()} {tag} {{ {member_display} }};".replace(f"{el_type_str.lower()}  {{", f"{el_type_str.lower()} {{")
                if not el_name: el_name = f"Anonymous{el_type_str}_L{el_line}"

        elif cursor.kind == clang.cindex.CursorKind.FUNCTION_DECL:
            el_type_str = "Function"
            el_return_type = cursor.result_type.spelling
            
            param_details = []
            for arg_cursor in cursor.get_arguments():
                param_type_str = arg_cursor.type.spelling
                param_name_str = arg_cursor.spelling or ""
                current_param_str = param_type_str
                if param_name_str:
                    current_param_str = f"{param_type_str} {param_name_str}"
                if current_param_str.strip():
                    param_details.append(current_param_str.strip())

            is_variadic = False
            if cursor.type.kind == clang.cindex.TypeKind.FUNCTIONPROTO:
                is_variadic = cursor.type.is_function_variadic()
            
            if not param_details and not is_variadic:
                param_details.append("void")
            
            if is_variadic:
                param_details.append("...")
            
            params_str = ", ".join(param_details)
            display_str = f"{el_return_type} {el_name}({params_str});"

        elif cursor.kind == clang.cindex.CursorKind.VAR_DECL:
            if cursor.semantic_parent.kind == clang.cindex.CursorKind.TRANSLATION_UNIT:
                el_type_str = "Constant (Variable)" if cursor.type.is_const_qualified() else "Global Variable"
        
        elif cursor.kind == clang.cindex.CursorKind.MACRO_DEFINITION:
            if el_name and el_name not in processed_macro_names: 
                el_type_str = "Constant (Macro)"
                if re.search(rf"#define\s+{re.escape(el_name)}\s*\(", raw_first_line):
                    el_type_str = "Macro (Function-like)"
                processed_macro_names.add(el_name)

        elif cursor.kind == clang.cindex.CursorKind.INCLUSION_DIRECTIVE:
            el_type_str = "Include"
            el_name = cursor.spelling 

        if el_type_str and el_name: 
            elements.append(CElement(base_file_name, el_name, el_type_str, 
                                     el_line, display_str, raw_first_line, el_return_type))
    return elements

def generate_markdown(elements: List[CElement], source_dir_name: str) -> str:
    md_lines: List[str] = [
        f"# API Documentation for C Files in `{source_dir_name}`",
        f"\nGenerated on: {time.strftime('%Y-%m-%d %H:%M:%S')}\n"
    ]

    if not elements:
        md_lines.append("No C elements found to document.")
        return "\n".join(md_lines)

    elements_by_file: Dict[str, List[CElement]] = {}
    for el in elements:
        elements_by_file.setdefault(el.file_name, []).append(el)

    global_cat_order = [
        "Constants", "Typedefs", "Enums", "Structs", "Unions", "Global Variables"
    ]
    global_type_to_cat_map = {
        "Constant (Macro)": "Constants",
        "Macro (Function-like)": "Constants",
        "Constant (Variable)": "Constants",
        "Typedef": "Typedefs",
        "Typedef Enum": "Typedefs",
        "Typedef Struct": "Typedefs",
        "Typedef Union": "Typedefs",
        "Enum": "Enums",
        "Struct": "Structs",
        "Union": "Unions",
        "Global Variable": "Global Variables"
    }

    sorted_file_names = sorted(elements_by_file.keys())
    for file_idx, file_name in enumerate(sorted_file_names):
        md_lines.append(f"## File: `{file_name}`\n")
        file_elements = elements_by_file[file_name]

        includes = sorted([el for el in file_elements if el.element_type == "Include"], key=lambda x: x.line)
        if includes:
            md_lines.append("### Includes")
            for el in includes:
                md_lines.append(f"* Line {el.line}: `{el.display_string}`")
            md_lines.append("")

        globals_by_cat: Dict[str, List[CElement]] = {cat: [] for cat in global_cat_order}
        has_any_globals = False
        for el in file_elements:
            cat = global_type_to_cat_map.get(el.element_type)
            if cat:
                globals_by_cat[cat].append(el)
                has_any_globals = True
        
        if has_any_globals:
            md_lines.append("### Global Declarations")
            for cat_name in global_cat_order:
                cat_elements = sorted(globals_by_cat[cat_name], key=lambda x: x.line)
                if cat_elements:
                    md_lines.append(f"#### {cat_name}")
                    for el in cat_elements:
                        md_lines.append(f"* Line {el.line}: `{el.display_string}`")
                    md_lines.append("")

        functions = sorted([el for el in file_elements if el.element_type == "Function"], 
                           key=lambda x: (x.return_type or "unknown", x.name, x.line))
        if functions:
            md_lines.append("### Functions")
            funcs_by_ret_type: Dict[str, List[CElement]] = {}
            for fn_el in functions:
                ret_type_key = fn_el.return_type if fn_el.return_type else "unknown_return_type"
                funcs_by_ret_type.setdefault(ret_type_key, []).append(fn_el)
            
            for ret_type_key in sorted(funcs_by_ret_type.keys()):
                h4_ret_type = ret_type_key.replace("*", "Ptr").replace(" ", "_").replace(":", "_") # Sanitize
                md_lines.append(f"#### {h4_ret_type} Functions")
                for el in funcs_by_ret_type[ret_type_key]:
                    md_lines.append(f"* Line {el.line}: `{el.display_string}`")
                md_lines.append("")
        
        if file_idx < len(sorted_file_names) - 1:
             md_lines.append("\n---") # Use triple dash for thematic break
    
    return "\n".join(md_lines)

def main():
    parser = argparse.ArgumentParser(description="Generate structured API documentation from C/H files.")
    parser.add_argument("directory", help="Directory containing .c and .h files.")
    parser.add_argument("-I", "--include", action="append", default=[], dest="include_paths", help="Additional include paths.")
    parser.add_argument("--libclang", dest="libclang_path", default=None, help="Path to libclang library or directory.")
    args = parser.parse_args()

    if args.libclang_path:
        if os.path.isfile(args.libclang_path): clang.cindex.Config.set_library_file(args.libclang_path)
        elif os.path.isdir(args.libclang_path): clang.cindex.Config.set_library_path(args.libclang_path)
        else: print(f"Warning: libclang path '{args.libclang_path}' not valid.", file=sys.stderr)

    target_dir = args.directory
    if not os.path.isdir(target_dir):
        print(f"Error: Directory '{target_dir}' not found.", file=sys.stderr); sys.exit(1)

    all_c_elements: List[CElement] = []
    files_to_parse = [os.path.join(target_dir, f) for f in os.listdir(target_dir) 
                      if os.path.isfile(os.path.join(target_dir, f)) and 
                         (f.lower().endswith(".c") or f.lower().endswith(".h"))]

    if not files_to_parse: print(f"No .c or .h files found in '{target_dir}'."); sys.exit(0)

    print(f"Scanning directory: {target_dir}")
    for file_path in files_to_parse:
        print(f"Parsing file: {os.path.abspath(file_path)}")
        all_c_elements.extend(extract_elements_from_file(file_path, args.include_paths))

    type_priority = { 
        "Typedef Enum": 1, "Typedef Struct": 1, "Typedef Union": 1,
        "Enum": 2, "Struct": 2, "Union": 2, "Function": 2,
        "Typedef": 3, "Macro (Function-like)": 3,
        "Constant (Macro)": 4, "Constant (Variable)": 4, "Global Variable": 4,
        "Include": 5
    }
    all_c_elements.sort(key=lambda el: (
        el.file_name, el.line, type_priority.get(el.element_type, 99), el.raw_first_line
    ))

    unique_line_keys = set()
    deduplicated_elements = []
    for el in all_c_elements:
        line_key = (el.file_name, el.line, el.raw_first_line) 
        if line_key not in unique_line_keys:
            unique_line_keys.add(line_key)
            deduplicated_elements.append(el)
    all_c_elements = deduplicated_elements
    
    if all_c_elements:
        dir_name = os.path.basename(os.path.abspath(target_dir))
        md_content = generate_markdown(all_c_elements, dir_name)
        output_file = os.path.join(target_dir, "api_documentation_structured.md")
        try:
            with open(output_file, "w", encoding="utf-8") as f: f.write(md_content)
            print(f"Structured documentation generated: {output_file}")
        except IOError as e: print(f"Error writing markdown: {e}", file=sys.stderr); sys.exit(1)
    else:
        print("No relevant C elements found to document.")

if __name__ == "__main__":
    main()
