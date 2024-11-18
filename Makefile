CC=time gcc
CFLAGS=-fstack-protector -fsanitize=address -Wall
LDFLAGS=
EXEC_PARSER = outputs/parser_test
EXEC_TOKENIZER = outputs/tokenizer_test
SRCS_PARSER = parser/parser.c
SRCS_TOKENIZER = tokenizer/tokenizer.c tokenizer/tokenizer_test.c


parse: #$(SRCS_PARSER)
	@echo "[+] Compiling Parser..."
	$(CC) $(CFLAGS) $(SRCS_PARSER) -o $(EXEC_PARSER)
	@echo "[+] Parser compiled successfully."
	@echo "[+] Running Parser..."
	./$(EXEC_PARSER)

tok: #$(SRCS_TOKENIZER)
	@echo "[+] Compiling Tokenizer..."
	$(CC) $(CFLAGS) $(SRCS_TOKENIZER) -o $(EXEC_TOKENIZER)
	@echo "[+] Tokenizer compiled successfully."
	@echo "[+] Running Tokenizer..."
	./$(EXEC_TOKENIZER)

clean:
	rm -f $(EXEC_PARSER) $(EXEC_TOKENIZER)
	@echo "[+] Cleaned up."