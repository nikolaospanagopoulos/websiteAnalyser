# web analyzer C++

## an application that analyzes websites and deduces their content

if you decide to build the application , you need mysql in your system. The database and the tables will be created automatically. Use the makefile to compile the app. The crow framework is also used to have access tou certain routes.

1. /analyze -> POST request , example {"website":"https://www.example.com"}
2. /addwords -> POST request to add words for checking in db, example {
   "theme":"robbery",
   "category":"crime"
   }
3. /deletecategory -> DELETE request, to delete a category and all its words from db.
