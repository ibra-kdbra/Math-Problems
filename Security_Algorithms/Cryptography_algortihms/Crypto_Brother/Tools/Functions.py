import os

def file_size(path, file):
    org_file=open(path+file, 'rb')
    org_file.seek(0, os.SEEK_END)
    file_size=org_file.tell() #FILE SIZE
    org_file.close
    return file_size
class switch:

	def __init__(self, variable, comparator=None, strict=False):
		self.variable = variable
		self.matched = False
		self.matching = False
		if comparator:
			self.comparator = comparator
		else:
			self.comparator = lambda x, y: x == y
		self.strict = strict

	def __enter__(self):
		return self

	def __exit__(self, exc_type, exc_val, exc_tb):
		pass

	def case(self, expr, break_=False):
		if self.strict:
			if self.matched:
				return False
		if self.matching or self.comparator(self.variable, expr):
			if not break_:
				self.matching = True
			else:
				self.matched = True
				self.matching = False
			return True
		else:
			return False

	def default(self):
		return not self.matched and not self.matching

if __name__ == "__main__":
    #USAGE OF FILE SIZE CALCULATION TOOL
    print(file_size(str(os.getcwd())+'\\' , "test_file.rar"))