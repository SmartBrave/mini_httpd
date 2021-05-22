use offer;

CREATE TABLE IF NOT EXISTS schedule (CompanyID int NOT NULL AUTO_INCREMENT PRIMARY KEY, CompanyName char(60) NOT NULL, NotDeliver bool NOT NULL , Deliverd bool NOT NULL, Exam bool NOT NULL, FirstInterview bool NOT NULL, SecondInterview bool NOT NULL, ThirdInterview bool NOT NULL, HRInterview bool NOT NULL, GetOffer bool NOT NULL, Note text , UserID int NOT NULL );
