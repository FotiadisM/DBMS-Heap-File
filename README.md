Όσο αναφορά τις σχεδιαστηκές επιλογές μου, σε κάθε block δεσμεύω 1 byte στο οποίο κρατάσω τον αριθμό τον records
που περιέχει το block

### Bonus:
    Παρατηρώ πως ο MRU έχει τα μισά read και lseek σε σχέση με την LRU και κατά συνέπεια τρέχει και στον μισό χρόνο.
    Θεωρώ πως αυτό συμβαίνει διότι στην TestFileScan το αρχείο "is being repeatedly scanned in a [Looping Sequential]
    reference pattern" κάτι στο οποίο ο MRU είναι ο ιδανικός replacement algorithm όπως αποδείχτηκε στην 11η VLDM συνενδρία
    απο τον Chou kai DeWitt 
