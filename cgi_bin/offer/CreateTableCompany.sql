use offer;

create table if not exists company (CompanyID int not null auto_increment primary key,CompanyName char(60) not null);

create table if not exists schedule( CompanyID int not null, UserID int not null, NotDeliver bool not null default false, Deliverd bool not null default false, Exam bool not null default false, FirstInterview bool not null default false, SecondInterview bool not null default false, ThirdInterview bool not null default false, HRInterview bool not null default false, GetOffer bool not null default false, Note text, primary key(CompanyID,UserID ));
