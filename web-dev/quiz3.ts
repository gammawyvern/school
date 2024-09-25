

interface Book {
  title: string;
  author: string;
  pages: number;
}

let books: Book[] = [
  { title: "1984", author: "George Orwell", pages: 328 },
  { title: "The Great Gatsby", author: "F. Scott Fitzgerald", pages: 180 },
  { title: "To Kill a Mockingbird", author: "Harper Lee", pages: 281 },
  { title: "Brave New World", author: "Aldous Huxley", pages: 264 },
  { title: "The Catcher in the Rye", author: "J.D. Salinger", pages: 234 },
];

let last2 = books.slice(-2);
console.log(last2);

let a = { title: "The Hobbit", author: "J.R.R. Tolkien", pages: 310 };
let b = { title: "Pride and Prejudice", author: "Jane Austen", pages: 432 };


let updated = books.splice(2, 1, a, b);
console.log();
console.log(books);

for(let book_i=0; book_i<books.length; book_i++) {
  console.log(`Book: ${book_i + 1}`);
  for(let prop in books[book_i]) {
    console.log(`${prop}: ${books[book_i][prop]}`);
  }

  console.log();
}

function splitIt(books: Book[]): [Book[], Book[]] {
  const [first, sec, ...rest] = books;
  return [[first, sec], rest];
}

const [firstTwo, theRest] = splitIt(books);
console.log("First Two Books:", firstTwo);
console.log("The Rest of the Books:", theRest);

