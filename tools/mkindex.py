#!/usr/bin/env python
import sys

def main(argv):
    import re, fileinput
    PAT = re.compile(r'<a\s+name="([^"]+)">([^<]+)</a>')
    for line in fileinput.input():
        for m in PAT.finditer(line):
            (name,title) = m.groups()
            print '<a href="#%s">[%s]</a>' % (name, title)
    return

if __name__ == '__main__': sys.exit(main(sys.argv))
