build:
	gcc tema1.c utils.h -g -o tema

clean:
	rm tema

checker:
	python3 checker.py
