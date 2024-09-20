import sys;
from vigenere import transform_text, decrypt_op;

cipher_text = "\
JUGZSLYIGZNZZNMYELBYMYEKVABETTTLHLDBESBXHMZTL\
KOIGEWIABEIGXEOVRRSOWPSBETAVPERYTEDWFNMPONBNH\
NIMJAUVANKIYLLGVWWRYAVYAVYKLNGZNZUOPETAVAOVNN\
VSXVHHNTAVSNESAZNXJBKZGAKLRZNMYEVZTRFNMYELKRX\
VTLNHXIEHECXNALGIMPMBJTXIBELELBYBJLBMIGXHXIEM\
FDTPHXPHXPMBJTXIBELELBYICETJEMVLELSPYYRFUARVX\
KOAZDXRWTPFHISHCOGXWAVRXUIWNEZFWKFNZYERKHXIEF\
ZSMVRUCUXNETIELFPEVALVDMFBXNIMYYHLLHFKTIONEDL\
VEPYAMPONUOXMEKPBHUYLDIEVSTKYHL\
";

cipher_text= cipher_text.lower();
key_size = int(sys.argv[1]);

########################################
# Here we are splitting the main cipher 
# text into individual cipher texts 
# based on key size
########################################

lists = [""] * key_size; 
for letter_index in range(len(cipher_text)):
    lists[letter_index % key_size] += cipher_text[letter_index]; 

########################################
# Here I am recording the frequency of 
# each letter in each split cipher text 
########################################

base = ord('a');
freq = [[0] * 26 for i in range(key_size)];

for segment_i in range(key_size):
    for char_i in range(len(lists[segment_i])):
        char = lists[segment_i][char_i];
        num_value = ord(char) - base;

        freq[segment_i][num_value] += 1;

########################################
# Here I am creating the key based on
# assuming that the most common letter 
# in each split cipher text is 'e'
########################################

key = "";
for segment_i in range(key_size):
    e = freq[segment_i].index(max(freq[segment_i]));
    offset = e - (ord('e') - base);
    char = chr((offset % 26) + base);
    key += char;

plain_text = transform_text(cipher_text, key, decrypt_op); 

print(f"{cipher_text}\n");
print(f"{key}\n");
print(f"{plain_text}\n");
