-- select id from student where id = 3213;
-- 
-- update student set MaximumPreferredEnrollment=null where Id='5123'
-- 
-- select transcript.UoSCode, unitofstudy.UoSName from transcript, unitofstudy where transcript.UoSCode=unitofstudy.UoSCode
-- and StudId='3213' and Year='2016' and Semester='Q1' and Grade is null;
-- 
-- SET AUTOCOMMIT=1;

drop procedure if exists course_withdraw;
delimiter |
create procedure course_withdraw(username int(11), scode char(8), quarter char(2), o_year int(11))
begin
	DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	start transaction;
	if ((select count(*) from transcript where (UoSCode = scode and Semester = quarter and Year = o_year))>0) then 
    delete from transcript where (UoSCode = scode and Semester = quarter and Year = o_year);
-- 		IF o_year = 2016 THEN
-- 			SIGNAL SQLSTATE '45000';
-- 		end if;
	update uosoffering set Enrollment=Enrollment-1  where (UoSCode = scode and Semester = quarter and Year = o_year);
    end if;
	commit;

end
|delimiter ;

drop procedure if exists course_enroll;
delimiter |
create procedure course_enroll(IN username int(11), IN scode char(8), IN quarter char(2), IN o_year int(11))
begin
	DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	start transaction;
	if(select count(*) from transcript where (UoSCode = scode and Semester = quarter and Year = o_year)=0) then
    insert into transcript values (username, scode, quarter, o_year, null);
-- 		IF o_year = 2016 THEN
-- 			SIGNAL SQLSTATE '45000';
-- 		end if;
	update uosoffering set Enrollment=Enrollment+1  where (UoSCode = scode and Semester = quarter and Year = o_year);
	end if;
	commit;
end
|delimiter ;


set @status = 'over';
drop trigger if exists c_status;
delimiter //
create trigger c_status after update on uosoffering
for each row 
begin 
	set @status = 'over';
	if ((new.Enrollment/new.MaxEnrollment)< 0.5) then 
	set @status = 'below';
    end if;
end;
//



-- delete from transcript where StudId=3213 and UoSCode='COMP2007' and year=2016 and Semester='Q2';
-- update uosoffering set Enrollment=Enrollment-1  where UoSCode=scode;
-- insert into transcript values (3213, 'COMP2007', 'Q2', 2016, null);
-- update uosoffering set Enrollment = 111 where UoSCode='COMP2007'and year=2016 and Semester='Q2';
-- call course_withdraw(3213, 'COMP2007', 'Q2', 2016);
-- call course_enroll(3213, 'COMP2007', 'Q2', 2016);
-- select count(*) from transcript where UoSCode = 'COMP2007' and Semester = 'Q2' and Year = 2016;
-- 
-- SHOW ENGINE INNODB STATUS
-- SHOW ENGINE INNODB MUTEX
-- SHOW ENGINE PERFORMANCE_SCHEMA STATUS
-- 
-- 
-- create table innodb_monitor(x int) engine=innodb;
-- 
-- create table innodb_lock_monitor(x int) engine=innodb;
-- 
-- create table innodb_table_monitor(x int) engine=innodb;
-- 
-- create table innodb_tablespace_monitor(x int) engine=innodb;
-- 
-- 
-- drop table innodb_monitor;
-- drop table innodb_lock_monitor;
-- drop table innodb_table_monitor;
-- drop table innodb_tablespace_monitor;
-- 
-- select @status;
-- update uosoffering set uosoffering.Enrollment = 2 where uosoffering.UoSCode = 'COMP2007' and uosoffering.Year = 2016;
-- update uosoffering set uosoffering.Enrollment = 111 where uosoffering.UoSCode = 'COMP2007' and uosoffering.Year = 2016;

-- 
-- 
-- select distinct uosoffering.UoSCode, unitofstudy.UoSName, uosoffering.Semester, uosoffering.Year,uosoffering.Enrollment,uosoffering.MaxEnrollment from 
-- uosoffering natural join unitofstudy natural join requires natural join classroom natural join lecture 
-- where ((uosoffering.Year=2016 and uosoffering.Semester='Q1') or (uosoffering.Year=2017 and uosoffering.Semester='Q2')) and MaxEnrollment>Enrollment and 
-- requires.PrereqUoSCode in (select UoSCode from transcript where StudId='3213' and (Grade='CR' or Grade='P')) 
-- and Type not in(select NonPreferredClassroomType from student where Id='3213') and uosoffering.UoSCode not in
-- (select UoSCode from transcript where StudId='3213') and MaxEnrollment<=(select MaximumPreferredEnrollment from student where Id='3213');