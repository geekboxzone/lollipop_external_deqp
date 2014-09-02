import argparse
import string

class TestGroup:
	def __init__(self, name, parent = None):
		self.parent = parent
		self.name = name
		self.testGroups = {}
		self.testCases = {}

		if parent:
			assert not name in parent.testGroups
			parent.testGroups[name] = self

	def getName (self):
		return self.name

	def getPath (self):
		if self.parent:
			return self.parent.getPath() + "." + self.name
		else:
			return self.name

	def hasGroup(self, groupName):
		return groupName in self.testGroups

	def getGroup(self, groupName):
		return self.testGroups[groupName]

	def hasTest(self, testName):
		return testName in self.testCases

	def getTest(self, testName):
		return self.testCases[testName]

	def hasTestCases(self):
		return len(self.testCases) != 0

	def hasTestGroups(self):
		return len(self.testGroups) != 0

	def getTestCases(self):
		return self.testCases.values()

	def getTestGroups(self):
		return self.testGroups.values()

class TestCase:
	def __init__(self, name, parent):
		self.name = name
		self.parent = parent

		assert not name in self.parent.testCases
		self.parent.testCases[name] = self

	def getPath (self):
		return self.parent.getPath() + "." + self.name

	def getName(self):
		return self.name

def addGroupToHierarchy(rootGroup, path):
	pathComponents = string.split(path, ".")
	currentGroup = rootGroup

	assert pathComponents[0] == rootGroup.getName()

	for i in range(1, len(pathComponents)):
		component = pathComponents[i]

		if currentGroup.hasGroup(component):
			currentGroup = currentGroup.getGroup(component)
		else:
			currentGroup = TestGroup(component, parent=currentGroup)

def addTestToHierarchy(rootGroup, path):
	pathComponents = string.split(path, ".")
	currentGroup = rootGroup

	assert pathComponents[0] == rootGroup.getName()

	for i in range(1, len(pathComponents)):
		component = pathComponents[i]

		if i == len(pathComponents) - 1:
			TestCase(component, currentGroup)
		else:
			if currentGroup.hasGroup(component):
				currentGroup = currentGroup.getGroup(component)
			else:
				currentGroup = TestGroup(component, parent=currentGroup)

def loadTestHierarchy (input):
	line = input.readline()
	rootGroup = None

	if line.startswith("GROUP: "):
		groupName	= line[len("GROUP: "):-1]
		rootGroup	= TestGroup(groupName)
	else:
		print(line)
		assert False

	for line in input:
		if line.startswith("GROUP: "):
			groupPath = line[len("GROUP: "):-1];
			addGroupToHierarchy(rootGroup, groupPath)
		elif line.startswith("TEST: "):
			testPath = line[len("TEST: "):-1]
			addTestToHierarchy(rootGroup, testPath)
		else:
			assert False

	return rootGroup

def writeAndroidCTSTest(test, output):
	output.write('<Test name="%s" />\n' % test.getName())

def writeAndroidCTSTestCase(group, output):
	assert group.hasTestCases()
	assert not group.hasTestGroups()

	output.write('<TestCase name="%s">\n' % group.getName())

	for testCase in group.getTestCases():
		writeAndroidCTSTest(testCase, output)

	output.write('</TestCase>\n')

def writeAndroidCTSTestSuite(group, output):
	if group.getName() == "performance":
		return;

	output.write('<TestSuite name="%s">\n' % group.getName())

	for childGroup in group.getTestGroups():
		if group.getName() == "performance":
			continue;

		if childGroup.hasTestCases():
			assert not childGroup.hasTestGroups()
			writeAndroidCTSTestCase(childGroup, output)
		elif childGroup.hasTestGroups():
			writeAndroidCTSTestSuite(childGroup, output)
		# \note Skips groups without testcases or child groups

	output.write('</TestSuite>\n')

def writeAndroidCTSFile(rootGroup, output, name="dEQP-GLES3", targetBinaryName="com.drawelements.deqp", appPackageName="com.drawelements.deqp.gles3"):
	output.write('<?xml version="1.0" encoding="UTF-8"?>\n')
	output.write('<TestPackage name="%s" targetBinaryName="%s" appPackageName="%s" testType="deqpTest">\n' % (name, targetBinaryName, appPackageName))

	writeAndroidCTSTestSuite(rootGroup, output)

	output.write('</TestPackage>\n')

if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument('input',                      type=argparse.FileType('r'),    help="Input dEQP test hierarchy in txt format.")
	parser.add_argument('output',                     type=argparse.FileType('w'),    help="Output file for Android CTS test file.")
        parser.add_argument('--name',     dest="name",    type=str,                       required=True, help="Name of the test package")
        parser.add_argument('--binary',   dest="binary",  type=str,   default="com.drawelements.deqp",   help="Target binary name")
        parser.add_argument('--package',  dest="package", type=str,                       required=True, help="Name of the app package")

	args = parser.parse_args()

	rootGroup = loadTestHierarchy(args.input)
	writeAndroidCTSFile(rootGroup, args.output, name=args.name, targetBinaryName=args.binary, appPackageName=args.package)
