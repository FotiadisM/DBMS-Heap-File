#ifndef HEAP_FILE_H
#define HEAP_FILE_H

typedef enum HP_ErrorCode {
  HP_OK,
  HP_ERROR
} HP_ErrorCode;

typedef struct Record {
	int id;
	char name[15];
	char surname[20];
	char city[20];
} Record;

/*
 * Η συνάρτηση HP_Init χρησιμοποιείται για την αρχικοποίηση του heap_file.
 * Σε περίπτωση που εκτελεστεί επιτυχώς, επιστρέφεται HP_OK, ενώ σε
 * διαφορετική περίπτωση κάποιος κωδικός λάθους.
 */
HP_ErrorCode HP_Init();

/*
 * Η συνάρτηση HP_CreateFile χρησιμοποιείται για τη δημιουργία και
 * κατάλληλη αρχικοποίηση ενός άδειου αρχείου σωρού με όνομα fileName.
 * Σε περίπτωση που εκτελεστεί επιτυχώς, επιστρέφεται HP_OK, ενώ σε
 * διαφορετική περίπτωση κάποιος κωδικός λάθους.
 */
HP_ErrorCode HP_CreateFile(
	const char *fileName		/* όνομααρχείου */
	);


/*
 * Η συνάρτηση HP_OpenFile ανοίγει το αρχείο με όνομα filename και διαβάζει
 * από το πρώτο μπλοκ την πληροφορία που αφορά το αρχείο σωρού. Επιστρέφει
 * στην μεταβλητή fileDesc τον αναγνωριστικό αριθμό ανοίγματος αρχείου, όπως
 * αυτός επιστράφηκε από το επίπεδο διαχείρισης μπλοκ. Σε περίπτωση που
 * εκτελεστεί επιτυχώς, επιστρέφεται HP_OK, ενώ σε διαφορετική περίπτωση
 * κάποιος κωδικός λάθους. Αν το αρχείο που ανοίχτηκε δεν πρόκειται για αρχείο
 * σωρού, τότε αυτό θεωρείται επίσης περίπτωση σφάλματος.
 */
HP_ErrorCode HP_OpenFile(
	const char *fileName, 		/* όνομα αρχείου */
  int *fileDesc             /* αναγνωριστικός αριθμός ανοίγματος αρχείου */
	);

/*
 * Η συνάρτηση HP_CloseFile κλείνει το αρχείο που προσδιορίζεται από τον
 * αναγνωριστικό αριθμό ανοίγματος fileDesc. Σε περίπτωση που εκτελεστεί
 * επιτυχώς, επιστρέφεται HP_OK, ενώ σε διαφορετική περίπτωση κάποιος
 * κωδικός λάθους.
 */
HP_ErrorCode HP_CloseFile(
	int fileDesc 		/* αναγνωριστικός αριθμός ανοίγματος αρχείου */
	);

/*
 * Η συνάρτηση HP_InsertEntry χρησιμοποιείται για την εισαγωγή μίας
 * εγγραφής στο αρχείο σωρού. Ο αναγνωριστικός αριθμός ανοίγματος του
 * αρχείου δίνεται με την fileDesc ενώ η εγγραφή προςεισαγωγή προσδιορίζεται
 * από τη δομή record. Η εγγραφή προστίθεται στο τέλος του αρχείου, μετά την
 * τρέχουσα τελευταία εγγραφή. Σε περίπτωση που εκτελεστεί επιτυχώς,
 * επιστρέφεται HP_OK, ενώ σε διαφορετική περίπτωση κάποιος κωδικός λάθους.
 */
HP_ErrorCode HP_InsertEntry(
	int fileDesc,		/* αναγνωριστικός αριθμός ανοίγματος αρχείου */
	Record record		/* δομή που προσδιορίζει την εγγραφή */
	);

/*
 * Η συνάρτηση HP_PrintAllEntries χρησιμοποιείται για την εκτύπωση όλων των
 * εγγραφών που υπάρχουν στο αρχείο σωρού. Το fileDesc είναι ο αναγνωριστικός
 * αριθμός ανοίγματος του αρχείου, όπως αυτός έχει επιστραφεί από το επίπεδο
 * διαχείρισης μπλοκ. Σε περίπτωση που εκτελεστεί επιτυχώς, επιστρέφεται HP_OK,
 * ενώ σε διαφορετική περίπτωση κάποιος κωδικός λάθους.
 */
HP_ErrorCode HP_PrintAllEntries(
	int fileDesc,		/* αναγνωριστικός αριθμός ανοίγματος αρχείου */
	char *attrName,		/* όνομα του πεδίου*/
	void *value         /* τιμή προς αναζήτηση */
	);

/*
 * Η συνάρτηση HP_GetEntry χρησιμοποιείται για την επιστροφή στην δομή record
 * της εγγραφής εκείνης που βρίσκεται στην rowId θέση στο αρχείο σωρού. Σε
 * περίπτωση που εκτελεστεί επιτυχώς, επιστρέφεται HP_OK, ενώ σε διαφορετική
 * περίπτωση κάποιος κωδικός λάθους.
 */
HP_ErrorCode HP_GetEntry(
  int fileDesc,		/* αναγνωριστικός αριθμός ανοίγματος αρχείου */
  int rowId,      /* θέση αγραφής στο αρχείο σωρού */
  Record *record  /* δομή στην οποία επιστρέφεται η εγγραφή */
  );

HP_ErrorCode HP_PrintUniversal(int fileDesc);

#endif // HEAP_FILE_H