# mini world
- small part of the real world
- that we want to represent in a db
- "snapshot" we are interested in
- example: school, library, bank, hospital

| mini world | include | dont include |
|-|-|-|
| school|students, teachers, courses | cafeteria menu, play ground equipment |
| library | books, members, transactions | building layout, cleaning schedule |
| hospital | patients, doctors, med | parking lot |
| bank | accounts, customers, loans, transactions | security camera footage|

# Entity Intro

- some real world object can be identified as entity
- table == entity == nouns
- entities: books, members , librarians, (objects)

## Activity:
Write 2 examples of miniworlds you encounter in daily life and list 3–5 things included in each.

### Example Solution:
- Grocery Store → Items, Customers, Cashiers, Bills, Orders.
- Gym → Members, Trainers, Equipment, Membership Plans

# Attributes
- properties of entity
- student $\rightarrow$ name, age, class, Roll No.

## data types for attributes

| Data Type | Example | Description|
|-|-|-|
| Text / String|  Name = “Ali”| Stores letters, words, or sentences.|
|Number / Integer|  Age = 18| Stores whole numbers only.|
|Decimal / Float|  Marks = 87.5| Stores numbers with decimals.|
| Date| Date of Birth = 20-SEP-1999| Stores dates in day/month/year format.|
| Boolean| IsMember = Yes| Stores True/False or Yes/No values.|

