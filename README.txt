dsh - Una semplice shell in C
Questo progetto è una semplice implementazione di una shell in linguaggio C. Il codice include funzionalità di base come l'esecuzione di comandi, la gestione dei percorsi, la ridirezione dell'output e l'uso di pipe.

Funzionalità
Esecuzione di comandi: la shell può eseguire comandi specificati nel percorso.
Gestione dei percorsi: è possibile impostare il percorso di ricerca dei comandi utilizzando il comando setpath.
Ridirezione dell'output: la shell supporta la ridirezione dell'output utilizzando i simboli > e >>.
Pipe: la shell supporta l'uso di pipe con il simbolo |.
Struttura del codice
Il codice è strutturato in diverse funzioni, ognuna delle quali gestisce una specifica funzionalità della shell:

panic: stampa un messaggio di errore e termina l'esecuzione.
prompt: stampa il prompt della shell e legge l'input dell'utente.
set_path: imposta il percorso di ricerca dei comandi.
path_lookup: cerca un comando nel percorso specificato.
rel2abs: converte un percorso relativo in un percorso assoluto.
do_redir: gestisce la ridirezione dell'output.
do_pipe: gestisce l'uso di pipe.
do_exec: esegue un comando.
main: funzione principale che gestisce il ciclo di vita della shell.
Come compilare ed eseguire
Per compilare il codice, utilizzare il comando gcc:

$gcc -o dsh dsh.c

Per eseguire la shell, utilizzare il comando:

./dsh

Note
Questo progetto è un esempio di come implementare una shell in C. Non include tutte le funzionalità di una shell completa e non è destinato ad essere utilizzato in un ambiente di produzione.