# insert_file.py

start_fs = 22 #no of sectors to skip before filesystem
end_fs_txt = start_fs +11
end_fs_art = end_fs_txt + 3
with open("build/os.img", "r+b") as f:
    for i in range(start_fs,end_fs_txt):
        f.seek(512*(i))
        f.write(b"\0BLANK")
        

   