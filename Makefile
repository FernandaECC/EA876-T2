
all: main main_p main_t run_linear run_processos run_threads plotar

plotar: plotar.py
	python3 plotar.py 

run_linear: main
	./main > plot_linear.txt
	
run_processos: main_p
		./main_p > plot_processo.txt



run_threads: main_t
		./main_t > plot_thread.txt




main: main.c imageprocessing.c
	gcc -omain main.c imageprocessing.c  -I./ -lfreeimage -lm
	
	

main_p: imageprocessing.c main_p.c
	gcc -omain_p main_p.c imageprocessing.c  -I./ -lfreeimage -lm
	
	
	
main_t: imageprocessing.c main_t.c
	gcc -omain_t main_t.c imageprocessing.c  -I./ -lfreeimage -lm -lpthread
	
	
clean:
	rm main main_p main_t plot_linear.txt plot_thread.txt plot_processo.txt
	
  	
