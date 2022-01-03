/*
 * Written by Malakai "Shaylin" C.
 *   12 - 31 - 2021
 *   
 *  A simple hash map implementation in C  
 *
 */

// A node type for each element in the hash_map
typedef struct {
  int val;       // val you stored at this key
  char trk;      // true if this node is not deleted
  char* key;     // raw string key being used
} hash_node;

// definition of the hash map type
typedef struct {
  unsigned long int sze;         // stores the number of nodes in map
  hash_node* map;  // array of hash_nodes in map
} hash_map;

// generates a hash_map of sze
//  if sze is less than 1, defaults to 1
hash_map gen_map(unsigned long int sze);

// A simple hash formula for strings
unsigned long int hash_key(char* key, unsigned long int sze); // returns hash of key 0 to sze-1

// Insert and delete nodes from map
//  both auto resize map for memory effeciency
//
// NOTE: del_node returns 1 if key nonexistint
char set_node(char* key, int val, hash_map* map); // put val at key in map
char del_node (char* key, hash_map* map); // del key from map

// return val at key in map
// CAUTION: exits program with EXIT_FAILURE if key noexistint
int get_node(char* key, hash_map* map); 

// frees pointers in map for map deletion
void clean_hash(hash_map* map); 

// Used to resize map based on factor
//  CAUTION: when using factor less than 1, nodes can deleted!
void resize_map(hash_map* map, float factor);
