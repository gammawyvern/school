import sys;

########################################
# Encrypt / Decrypt functions
########################################

def transform_text(text, key, op):
    transformed_text = ""; 

    for index, letter in enumerate(text):
        if not letter.isalpha():
            continue;

        x = ord(letter) - ord('a');
        y = ord(key[index % len(key)]) - ord('a');
        transformed_text += chr((op(x, y) % 26) + ord('a'));

    return transformed_text;

def encrypt_op(x, y):
    return x + y;

def decrypt_op(x, y):
    return x - y;



########################################
# Checking inputs and running
########################################
if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python script.py [option] [text] [key]");
        sys.exit();

    op = sys.argv[1].lower();
    text = sys.argv[2].lower().replace(" ", "");
    key = sys.argv[3].lower();

    if not text.isalpha():
        print("Text must only contain alphabetic characters and spaces");
        sys.exit();
    if not key.isalpha():
        print("Key must only contain alphabetic characters");
        sys.exit();

    if op == "encrypt" or op == "e":
        print(transform_text(text, key, encrypt_op));
    elif op == "decrypt" or op == "d":
        print(transform_text(text, key, decrypt_op));
    else:
        print("\nInvalid operation. Use [encrypt | e] or [decrypt | d]\n")

