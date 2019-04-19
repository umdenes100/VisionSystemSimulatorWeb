from pycparser import parse_file, c_parser, c_generator
from pycparser.c_ast import FuncDef, FuncCall, ID, Compound, EmptyStatement

ast = parse_file('simple_program.c', use_cpp=True)

for i, item in enumerate(ast.ext):
	if isinstance(item, FuncDef) and item.body.block_items is not None:
		block_items = []
		for bi in item.body.block_items:
			block_items.append(bi)
			block_items.append(FuncCall(name=ID(name='test'), args=None))
		item.body.block_items = block_items

print(ast)
print()

generator = c_generator.CGenerator()
print(generator.visit(ast))
