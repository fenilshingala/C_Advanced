void encode( char const * plaintext, char* encryptedtext, int *num_bits_used );
void decode( char const* ciphertext, int num_chars, char* plaintext );

/* helper function for debugging */
void print_bits( char* buffer, int start_pos, int how_many );
