var books = [
    { title: "1984", author: "George Orwell", pages: 328 },
    { title: "The Great Gatsby", author: "F. Scott Fitzgerald", pages: 180 },
    { title: "To Kill a Mockingbird", author: "Harper Lee", pages: 281 },
    { title: "Brave New World", author: "Aldous Huxley", pages: 264 },
    { title: "The Catcher in the Rye", author: "J.D. Salinger", pages: 234 },
];
var last2 = books.slice(-2);
console.log(last2);
var a = { title: "The Hobbit", author: "J.R.R. Tolkien", pages: 310 };
var b = { title: "Pride and Prejudice", author: "Jane Austen", pages: 432 };
var updated = books.splice(2, 1, a, b);
console.log();
console.log(books);
for (var book_i = 0; book_i < books.length; book_i++) {
    console.log("Book: ".concat(book_i + 1));
    for (var prop in books[book_i]) {
        console.log("".concat(prop, ": ").concat(books[book_i][prop]));
    }
    console.log();
}
function splitIt(books) {
    var first = books[0], sec = books[1], rest = books.slice(2);
    return [[first, sec], rest];
}
var _a = splitIt(books), firstTwo = _a[0], theRest = _a[1];
console.log("First Two Books:", firstTwo);
console.log("The Rest of the Books:", theRest);
