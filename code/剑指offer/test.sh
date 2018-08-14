
# 取出文本和tid
import re
write_file = open("./content.txt", 'w')
pattern = re.compile(r'^(\d+),.*,\d+,(.*)\n$')
with open("test.txt", encoding='utf-8') as f:
    for line in f.readlines():
        r = pattern.match(line)
        if r:
#             print(r.group(1)+ "dasda" + r.group(2))
            write_file.write(r.group(1)+ "\t" + r.group(2) + '\n')
write_file.close()