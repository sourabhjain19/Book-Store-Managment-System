PRAGMA foreign_keys=OFF;

BEGIN TRANSACTION;
CREATE TABLE shop(
shopname text,
shopno int,
address text,
owner text,
password text);

INSERT INTO shop VALUES('KLE Bookstall',11,'Vidyanagar, Hubli','Sourabh Jain','Admin');
CREATE TABLE items(
name text,
price float,
units int,
type text);

CREATE TABLE customer(
id int,
name text);
CREATE TABLE orders(
no int primary key,
id int,
c_id int);

COMMIT;
