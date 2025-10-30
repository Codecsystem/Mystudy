import os

# 根目录
root = r"E:\Data\Work\algorithm_study\Mystudy"
# 输出文件
output_file = os.path.join(root, "all_code.md")

def escape_title(name: str) -> str:
    """去掉后缀，只保留文件名用于标题"""
    return os.path.splitext(name)[0]

with open(output_file, "w", encoding="utf-8") as out:
    # 遍历一级子目录（DataStruct, Graph, 数论...）
    for folder in sorted(os.listdir(root)):
        folder_path = os.path.join(root, folder)
        if not os.path.isdir(folder_path):
            continue  # 跳过非文件夹

        # 写目录标题
        out.write(f"# {folder}\n")

        # 遍历该目录下的 cpp 文件
        cpp_files = [f for f in os.listdir(folder_path) if f.endswith(".cpp")]
        for fname in sorted(cpp_files, key=lambda x: escape_title(x)):
            fpath = os.path.join(folder_path, fname)
            title = escape_title(fname)
            out.write(f"**{title}**\n")
            out.write("```cpp\n")

            with open(fpath, "r", encoding="utf-8", errors="ignore") as fin:
                out.write(fin.read())

            out.write("\n```\n\n")
