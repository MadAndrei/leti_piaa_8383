import os
for test_num, test_name in enumerate(os.listdir('Tests')):
	print('Run test number', test_num)
	print(''.join([line for line in open('Tests/' + test_name, 'r')]))
	os.system('./lab3 < ./Tests/' + test_name)
	print()