CREATE DATABASE IF NOT EXISTS pupper;
CREATE USER 'pupper'@'localhost' IDENTIFIED BY 'P455w0rd#';
GRANT ALL on pupper.* to 'pupper'@'localhost';

USE pupper;

CREATE TABLE persistent (id INT, callers INT, quote INT);

CREATE TABLE caller (
  id INT,
  username VARCHAR(12),
  password VARCHAR(12),
  email VARCHAR(80),
  date1 DATE,
  date2 DATE,
  date3 DATE,
  date4 DATE,
  topic TINYINT,
  calls INT,
  PRIMARY KEY (id));

CREATE TABLE message (
  id INT,
  sender VARCHAR(12),
  recipient VARCHAR(12),
  subject VARCHAR(36),
  datetime DATETIME,
  topic TINYINT,
  message TEXT,
  PRIMARY KEY (id));

CREATE TABLE file (
  id INT,
  filename VARCHAR(13),
  description VARCHAR(40),
  PRIMARY KEY (id));

CREATE TABLE quotes (
  id INT,
  quote TEXT,
  PRIMARY KEY (id));
  
  

