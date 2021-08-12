#!/bin/sh

python - << EOF

import subprocess
import os
import re

sfiles = []

for subdir, dirs, files in os.walk('.'):
	sfiles += [subdir+'/'+f for f in files if re.match(".*\.(cpp|qml|cc|h|js)$", f)]

xgettext = (os.getenv("XGETTEXT") or "xgettext").split()
outdir = (os.getenv("podir") or "po") + "/tok.pot"

subprocess.check_output(xgettext + sfiles + ['-o', outdir])

EOF
