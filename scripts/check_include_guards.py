# -*- coding: utf-8 -*-

import os
import sys
from fnmatch import fnmatch
from optparse import OptionParser

HEADER_PATTERNS		= ["*.hpp", "*.h"]
CHECK_END_COMMENT	= True

def getIncludeGuardName (headerFile):
	return '_' + os.path.basename(headerFile).upper().replace('.', '_')

def hasValidIncludeGuard (headerFile):
	includeGuard	= getIncludeGuardName(headerFile)
	f				= open(headerFile, 'rb')
	isHpp			= headerFile[-4:] == ".hpp"

	line0 = f.readline().strip()
	line1 = f.readline().strip()

	if line0 != ("#ifndef %s" % includeGuard):
		return False
	if line1 != ("#define %s" % includeGuard):
		return False

	if CHECK_END_COMMENT:
		lastLine		= ""
		expectedComment	= ("#endif // %s" if isHpp else "#endif /* %s */") % includeGuard
		for line in f:
			lastLine = line.strip()

		if lastLine != expectedComment:
#			print "'%s' != '%s'" % (lastLine, expectedComment)
			return False

	f.close()
	return True

def fixIncludeGuard (headerFile):
	f				= open(headerFile, 'rb')
	lines			= []
	isHpp			= headerFile[-4:] == ".hpp"
	includeGuard	=  getIncludeGuardName(headerFile)

	for line in f:
		lines.append(line)
	f.close()

	# Replace include guards
	lines[0] = "#ifndef %s\n" % includeGuard
	lines[1] = "#define %s\n" % includeGuard

	if CHECK_END_COMMENT:
		lines[len(lines)-1] = ("#endif // %s\n" if isHpp else "#endif /* %s */\n") % includeGuard

	f = open(headerFile, 'wb')
	for line in lines:
		f.write(line)
	f.close()

def isHeader (filename):
	for pattern in HEADER_PATTERNS:
		if fnmatch(filename, pattern):
			return True
	return False

def getHeaderFileList (path):
	headers = []
	if os.path.isfile(path):
		if isHeader(path):
			headers.append(path)
	else:
		for root, dirs, files in os.walk(path):
			for file in files:
				if isHeader(file):
					headers.append(os.path.join(root, file))
	return headers

if __name__ == "__main__":
	parser = OptionParser()
	parser.add_option("-x", "--fix", action="store_true", dest="fix", default=False, help="attempt to fix include guards (use with caution)")

	(options, args)	= parser.parse_args()
	fix				= options.fix
	headers			= []
	invalidHeaders	= []

	for dir in args:
		headers += getHeaderFileList(os.path.normpath(dir))

	print "Checking..."
	for header in headers:
		print "  %s" % header
		if not hasValidIncludeGuard(header):
			invalidHeaders.append(header)

	print ""
	if len(invalidHeaders) > 0:
		print "Found %d files with invalid include guards:" % len(invalidHeaders)

		for header in invalidHeaders:
			print "  %s" % header

		if not fix:
			sys.exit(-1)
	else:
		print "All headers have valid include guards."

	if fix:
		print ""
		for header in invalidHeaders:
			fixIncludeGuard(header)
			print "Fixed %s" % header
