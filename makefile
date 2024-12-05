assembler: assembler.o pre_assembler.o handle_macros.o table_of_macros.o first_pass.o encode_first_pass.o help_methods_for_first_pass.o table_of_labels.o check_errors_in_first_pass.o check_errors_in_second_pass.o print_messages.o second_pass.o encode_second_pass.o create_output_files.o help_methods.o
	gcc -g -Wall -ansi -pedantic assembler.o pre_assembler.o handle_macros.o table_of_macros.o first_pass.o encode_first_pass.o help_methods_for_first_pass.o table_of_labels.o check_errors_in_first_pass.o check_errors_in_second_pass.o print_messages.o second_pass.o encode_second_pass.o create_output_files.o help_methods.o -o assembler

assembler.o: assembler.c pre_assembler/pre_assembler.h pre_assembler/macros_structure/table_of_macros.h first_pass/first_pass.h labels_structure/table_of_labels.h second_pass/second_pass.h enums/errors.h enums/boolean.h errors_handling/print_messages.h util/create_output_files.h util/help_methods.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

# pre assembler files
pre_assembler.o: pre_assembler/pre_assembler.c pre_assembler/pre_assembler.h pre_assembler/handle_macros.h pre_assembler/macros_structure/table_of_macros.h enums/boolean.h enums/errors.h errors_handling/print_messages.h util/help_methods.h
	gcc -c -Wall -ansi -pedantic pre_assembler/pre_assembler.c -o pre_assembler.o

handle_macros.o: pre_assembler/handle_macros.c pre_assembler/handle_macros.h pre_assembler/macros_structure/table_of_macros.h enums/boolean.h enums/errors.h errors_handling/print_messages.h util/help_methods.h
	gcc -c -Wall -ansi -pedantic pre_assembler/handle_macros.c -o handle_macros.o

table_of_macros.o: pre_assembler/macros_structure/table_of_macros.c pre_assembler/macros_structure/table_of_macros.h enums/boolean.h enums/errors.h util/help_methods.h
	gcc -c -Wall -ansi -pedantic pre_assembler/macros_structure/table_of_macros.c -o table_of_macros.o

# first pass files
first_pass.o: first_pass/first_pass.c first_pass/first_pass.h first_pass/encode_first_pass.h first_pass/help_methods_for_first_pass.h labels_structure/table_of_labels.h enums/errors.h enums/warnings.h enums/boolean.h enums/labels.h enums/guidance.h errors_handling/print_messages.h util/help_methods.h pre_assembler/macros_structure/table_of_macros.h
	gcc -c -Wall -ansi -pedantic first_pass/first_pass.c -o first_pass.o

encode_first_pass.o: first_pass/encode_first_pass.c first_pass/encode_first_pass.h first_pass/help_methods_for_first_pass.h enums/address_method.h enums/operand_type.h enums/registers.h enums/encoding_type.h enums/commands.h util/help_methods.h
	gcc -c -Wall -ansi -pedantic first_pass/encode_first_pass.c -o encode_first_pass.o

help_methods_for_first_pass.o: first_pass/help_methods_for_first_pass.c first_pass/help_methods_for_first_pass.h enums/guidance.h enums/commands.h enums/registers.h enums/boolean.h enums/operand_type.h enums/address_method.h
	gcc -c -Wall -ansi -pedantic first_pass/help_methods_for_first_pass.c -o help_methods_for_first_pass.o

# labels structure files
table_of_labels.o: labels_structure/table_of_labels.c labels_structure/table_of_labels.h enums/labels.h enums/boolean.h enums/errors.h enums/guidance.h util/help_methods.h
	gcc -c -Wall -ansi -pedantic labels_structure/table_of_labels.c -o table_of_labels.o

# errors handling files
check_errors_in_first_pass.o: errors_handling/check_errors_in_first_pass.c errors_handling/check_errors_in_first_pass.h errors_handling/print_messages.h enums/errors.h enums/boolean.h enums/guidance.h enums/commands.h enums/registers.h enums/address_method.h enums/operand_type.h first_pass/help_methods_for_first_pass.h util/help_methods.h labels_structure/table_of_labels.h pre_assembler/macros_structure/table_of_macros.h
	gcc -c -Wall -ansi -pedantic errors_handling/check_errors_in_first_pass.c -o check_errors_in_first_pass.o

check_errors_in_second_pass.o: errors_handling/check_errors_in_second_pass.c errors_handling/check_errors_in_second_pass.h errors_handling/print_messages.h enums/errors.h enums/labels.h enums/boolean.h labels_structure/table_of_labels.h
	gcc -c -Wall -ansi -pedantic errors_handling/check_errors_in_second_pass.c -o check_errors_in_second_pass.o

print_messages.o: errors_handling/print_messages.c errors_handling/print_messages.h enums/errors.h enums/warnings.h
	gcc -c -Wall -ansi -pedantic errors_handling/print_messages.c -o print_messages.o

# second pass files
second_pass.o: second_pass/second_pass.c second_pass/second_pass.h second_pass/encode_second_pass.h labels_structure/table_of_labels.h errors_handling/check_errors_in_second_pass.h enums/boolean.h enums/errors.h enums/guidance.h enums/labels.h enums/operand_type.h util/help_methods.h first_pass/help_methods_for_first_pass.h first_pass/encode_first_pass.h first_pass/first_pass.h enums/commands.h enums/registers.h enums/address_method.h enums/encoding_type.h
	gcc -c -Wall -ansi -pedantic second_pass/second_pass.c -o second_pass.o

encode_second_pass.o: second_pass/encode_second_pass.c second_pass/encode_second_pass.h enums/encoding_type.h enums/labels.h
	gcc -c -Wall -ansi -pedantic second_pass/encode_second_pass.c -o encode_second_pass.o

# util files
create_output_files.o: util/create_output_files.c util/create_output_files.h enums/boolean.h enums/errors.h enums/guidance.h enums/operand_type.h enums/labels.h errors_handling/print_messages.h labels_structure/table_of_labels.h
	gcc -c -Wall -ansi -pedantic util/create_output_files.c -o create_output_files.o

help_methods.o: util/help_methods.c util/help_methods.h enums/errors.h enums/boolean.h errors_handling/print_messages.h
	gcc -c -Wall -ansi -pedantic util/help_methods.c -o help_methods.o