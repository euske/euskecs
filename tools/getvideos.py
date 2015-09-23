#!/usr/bin/env python
import sys

def main(argv):
    import re, fileinput
    PAT0 = re.compile(r'name="playlist_id"\s+value="([^"]*)"')
    PAT1 = re.compile(r'\bdata-video-id="([^"]*)"')
    PAT2 = re.compile(r'\bdata-title="([^"]*)"')
    plid = None
    (width,height) = (560,315)
    for line in fileinput.input():
        line = line.decode('utf-8').strip()
        m0 = PAT0.search(line)
        if m0:
            (plid,) = m0.groups()
        m1 = PAT1.search(line)
        m2 = PAT2.search(line)
        if m1 and m2:
            (vid,) = m1.groups()
            (title,) = m2.groups()
            #print (vid,title)
            print '<div class=embed>'
            print '<--', title.encode('utf-8'), '-->'
            src = 'https://www.youtube.com/embed/%s?list=%s' % (vid, plid)
            print '<iframe width="%s" height="%s" src="%s" frameborder="0" allowfullscreen></iframe>' % (width, height, src)
            print '</div>'
            print
    return

if __name__ == '__main__': sys.exit(main(sys.argv))
