
#include <mysql.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
using namespace std;

MYSQL *conn; /*pointer to connection handler*/

void login_menu();
void student_menu(string username);
void course_list(string username);
void transcript(string username);
void enroll(string username);
void withdraw(string username);
void personal_details(string username);
void SetMaximumPreferredEnrollment(string username);
void SetNonPreferredClassroomType(string username);




int main(int argc, const char * argv[]) {

    conn=mysql_init(NULL);
    //open connection
    mysql_real_connect(conn, "localhost", "root", "1234", "project3-nudb",0, NULL, CLIENT_MULTI_RESULTS);
//    if (conn == NULL)
//    {
//        //unable to connect
//        printf("Oh Noes!\n");
//    }
//    else
//    {
//        printf("You are now connected. Welcome!\n");
//    }

	mysql_query(conn,"delimiter // create trigger enroll_warn after update on uosoffering for each row begin if (new.Enrollment/new.MaxEnrollment)< 0.5 then SIGNAL SQLSTATE 'HY000' SET MESSAGE_TEXT = 'warning'; end if;end;");

    login_menu();
    
//close connection
    mysql_close(conn);
    
}

void login_menu(){
        while(1){
            MYSQL_RES *res_set;
            MYSQL_ROW row;
            mysql_query(conn,"SELECT * FROM student;");
            
            res_set = mysql_store_result(conn);
            
            int numrows=(int)mysql_num_rows(res_set);
            printf("Welcome!\n");
            printf("1. Login\n");
            printf("0. Exit\n");
            string  choice;
            cin>>choice;
            if(choice.length()>1){
                cout<<"please enter valid choice!"<<endl;
                continue;
            }
            char c=choice[0];
            if (c<'0'||c>'1') {
                cout<<"Please enter a valid choice!"<<endl;
                continue;
            }
            if(c=='0') {
                printf("Bye~\n");
                break;
            }
            printf("Please enter you username: ");
            string username;
            cin>>username;
            string password;
            printf("Please entern your password: ");
            cin>>password;
                        //determine whether the username and password are valid
            bool flag=false;
            for (int i=0;i<numrows; i++) {
                row=mysql_fetch_row(res_set);
                if (row!=NULL) {
                    
                    if(username==row[0]&& password==row[2])
                    {
                        flag=true;
                        break;
                    }
                    //            cout<<"ID: "<<row[0]<<endl;
                    //            cout<<"Name: "<<row[1]<<endl;
                }
            }
            if(flag==false) cout<<"Wrong username or password!"<<endl;
            else student_menu(username);           
			mysql_free_result(res_set);
            
        }
}




void student_menu(string username){
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	string sql="SELECT Name FROM student where Id = "+username;
 
 
	
	
	mysql_query(conn,sql.c_str());


            
    res_set = mysql_store_result(conn);
    
    row=mysql_fetch_row(res_set);
    cout<<"Hello, "<<row[0]<<endl;
    cout<<"What do you want to do?"<<endl;
    cout<<endl;
    while (1) {
        printf("1. Courses List\n");
        printf("2. Transcript\n");
        printf("3. Enroll\n");
        printf("4. Withdraw\n");
        printf("5. Personal Details\n");
        printf("0. Logout\n");
        string choice;
        cin>>choice;
        if(choice.length()>1){
            cout<<"please enter valid choice!"<<endl;
            continue;
        }
        char c=choice[0];
      
        if (c!='0'&&c!='1'&&c!='2'&&c!='3'&&c!='4'&&c!='5') {
            cout<<"please enter valid choice!"<<endl;
            continue;
        }
        
        if(c=='0') return;
        switch (c) {
            case '1':
                course_list(username);
                break;
            case '2':
                transcript(username);
                break;
            case '3':
                enroll(username);
                break;
            case '4':
                withdraw(username);
                break;
            case '5':
                personal_details(username);
                break;
            default:
                
                break;
        }
    }
    
}

void course_list(string username){
    printf("***********Course List***********\n");
    time_t t = time(0); 
    char year[5];
    char month[10];
    char month_num[3];
    string quarter;
    int num;

    
    strftime(year, sizeof(year), "%Y",localtime(&t)); 
    strftime(month, sizeof(month), "%B",localtime(&t)); 
    strftime(month_num, sizeof(month_num), "%m",localtime(&t));
	 
    cout<<"It's "<<month<<", "<<year<<endl;
    cout<<"You have taking the courses below in this quarter"<<endl;
    cout<<endl;
    string  choice;
    
    num = atoi(month_num);

    if (num>=1&&num<=3){quarter = "Q2";
	}else if(num>=4&&num<=6){quarter = "Q3";
	}else if(num>=7&&num<=9){quarter = "Q4";
	}else (quarter = "Q1");
//	cout<<quarter<<endl;
    
   
    
    
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	string sql="SELECT * FROM transcript natural join  unitofstudy where Studid = ";
 
 
	sql= sql + username + " and Semester = " +"'"+ quarter+"'"" and Year = " +"'"+ year +"'";
//	cout<<sql<<endl;
	
	mysql_query(conn,sql.c_str());
	mysql_error(conn); 

            
    res_set = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(res_set);
	for(int i = 0;i<numrows;i++){
		row = mysql_fetch_row(res_set);
		if(row != NULL){
			cout<<"UoSCode: "<<row[0]<< " UoSName:"<<row[6]<<endl;
		}
	}
	mysql_free_result( res_set );
	cout<<endl;
	printf("Press 0 to go back\n");
    while(1){
        cin>>choice;
        if(choice.length()>1){
            cout<<"please enter valid choice!"<<endl;
            continue;
        }
        char c=choice[0];
        if(c=='0') break;
    }
    
}

void transcript(string username){
    printf("***********Transcript***********\n");
    cout<<endl;
    
    MYSQL_RES *res_set;
	MYSQL_ROW row;
	string sql="SELECT UoSCode,ifnull(Grade,'NULL') FROM transcript where Studid = ";
 
 
	sql= sql + username;
//	cout<<sql<<endl;
	
	mysql_query(conn,sql.c_str());
	mysql_error(conn); 

            
    res_set = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(res_set);
	for(int i = 0;i<numrows;i++){
		row = mysql_fetch_row(res_set);
		if(row[0] != NULL){
			cout<<"UoSCode: "<<row[0]<< " Grade: "<<row[1]<<endl;
		}
	}
	
	cout<<endl;
    
    
    
    
    string  choice;
    
    while(1){
    	cout<<endl;
    	printf("If you want more details of any courses,please enter the course number.\n");
		printf("Enter 0 to go back Student Menu.\n");
        cin>>choice;
        cout<<endl;
        char c=choice[0];
        bool flag = false;
        if(c=='0') {
		break;
		}else {
			MYSQL_RES *res_set;
			MYSQL_ROW row;
			string sql="SELECT UoSCode,ifnull(Grade,'NULL') FROM transcript where Studid = ";
 
 
			sql= sql + username;
			//	cout<<sql<<endl;
	
			mysql_query(conn,sql.c_str());
			mysql_error(conn); 
			res_set = mysql_store_result(conn);
			for(int i = 0;i<numrows;i++){
				row = mysql_fetch_row(res_set);
				if(row[0] == choice){
					flag = true;
				}
			}
			if (!flag){
				cout<<"please enter valid choice!"<<endl;
            	continue;
			}else{
				MYSQL_RES *res_set;
				MYSQL_ROW row;
				string sql="select UoSCode,UoSName,Year,case when Semester = 'Q1' then 'Fall' when Semester = 'Q2' then 'Winter' when Semester = 'Q3' then 'Spring' when Semester = 'Q4' then 'Summer' end as Semester,Enrollment,MaxEnrollment,Name,ifnull(Grade,'Null') from ((transcript natural join uosoffering natural join unitofstudy) join faculty on Id = InstructorId)  where StudId = ";
 
 
				sql= sql + username + " and UoSCode = " +"'"+ choice+"'" + ";";
					//cout<<sql<<endl;
	
				mysql_query(conn,sql.c_str());
				mysql_error(conn); 

            
    			res_set = mysql_store_result(conn);
				int numrows = (int)mysql_num_rows(res_set);
				for(int i = 0;i<numrows;i++){
					row = mysql_fetch_row(res_set);
					if(row[0] != NULL){
						cout<<"Course: "<<row[0]<<endl;
						cout<<"UoSName: "<<row[1]<<endl;
						cout<<"Year: "<<row[2]<<endl;
						cout<<"Quarter: "<<row[3]<<endl;
						cout<<"Enrolled Students: "<<row[4]<<endl;
						cout<<"Maximum Enrollment: "<<row[5]<<endl;
						cout<<"Lecturer: "<<row[6]<<endl;
						cout<<"Grade: "<<row[7]<<endl;
					}
				}
			}
            
        }
        
    }
    mysql_free_result( res_set );
}
void enroll(string username){
	while(1){
    printf("***********Enroll Menu***********\n");
    
    time_t t = time(0);
    char year[5];
    char month[10];
    char month_num[3];
    string quarter;
    int num;
    int nextyear;
    string nextquarter;
    
    strftime(year, sizeof(year), "%Y",localtime(&t));
    strftime(month, sizeof(month), "%B",localtime(&t));
    strftime(month_num, sizeof(month_num), "%m",localtime(&t));
    num = atoi(month_num);
    
    if (num>=1&&num<=3){
        quarter = "Q2";
    }
    else if(num>=4&&num<=6){
        quarter = "Q3";
    }
    else if(num>=7&&num<=9){
        quarter = "Q4";
    }
    else{
        quarter = "Q1";
    }
    if (quarter=="Q1") {
        nextquarter="Q2";
        nextyear=atoi(year)+1;
    }
    else if(quarter=="Q2"){
        nextquarter="Q3";
        nextyear=atoi(year);
    }
    else if(quarter=="Q3"){
        nextquarter="Q4";
        nextyear=atoi(year);
    }
    else{
        nextquarter="Q1";
        nextyear=atoi(year);
    }
    string yearstr=year;


//	char buffer[5];   
//	_itoa( nextyear, buffer, 10 );  
//	string nextyearstr(buffer);    
    string nextyearstr = to_string(nextyear);
    cout<<"It's "<<month<<", "<<yearstr<<", quarter: "<<quarter<<endl;
    cout<<"Next quarter is "<<nextyearstr<<", "<<nextquarter<<endl;
    
    

        string query="select distinct UoSCode, UoSName, Semester,Year,Enrollment,MaxEnrollment from uosoffering natural join unitofstudy  natural join classroom natural join lecture left outer join requires using (UoSCode) where ((uosoffering.Year="+yearstr+" and uosoffering.Semester='"+quarter+"') or (uosoffering.Year="+nextyearstr+" and uosoffering.Semester='"+nextquarter+"')) and MaxEnrollment>Enrollment and (requires.PrereqUoSCode in (select UoSCode from transcript where StudId='"+username+"' and (Grade='CR' or Grade='P'))) and uosoffering.UoSCode not in (select UoSCode from transcript where StudId='"+username+"') and Type in(select distinct Type from classroom where Type != (ifnull((select NonPreferredClassroomType from student where Id='"+username+"'),'1'))) and MaxEnrollment<=(select ifnull(MaximumPreferredEnrollment,999999) from student where Id='"+username+"');";
//        cout<<"query: "<<query<<endl;
        string query2="select distinct UoSCode, UoSName, Semester,Year,Enrollment,MaxEnrollment from uosoffering natural join unitofstudy  natural join classroom natural join lecture where ((uosoffering.Year="+yearstr+" and uosoffering.Semester='"+quarter+"') or (uosoffering.Year="+nextyearstr+" and uosoffering.Semester='"+nextquarter+"')) and MaxEnrollment>Enrollment and uosoffering.UoSCode not in (select UoSCode from transcript where StudId='"+username+"') and MaxEnrollment<=(select ifnull(MaximumPreferredEnrollment,999999) from student where Id='"+username+"') and uosoffering.UoSCode not in(select distinct UoSCode from requires) and Type in(select distinct Type from classroom where Type != (ifnull((select NonPreferredClassroomType from student where Id='"+username+"'),'1')));";
//    string zwxssb = "select uosoffering.UoSCode from uosoffering";
//    cout<<"Query: "<<zwxssb<<endl;
    //    cout<<query<<endl;
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    mysql_query(conn, query.c_str());
    res_set=mysql_store_result(conn);
        MYSQL_RES *res_set2;
        MYSQL_ROW row2;
        mysql_query(conn, query2.c_str());
        res_set2=mysql_store_result(conn);
    if (res_set==NULL) {
        cout<<"res_set is null!"<<endl;
    }
    
    int numrows=(int)mysql_num_rows(res_set);
    int numrows2=(int)mysql_num_rows(res_set2);
    if ((numrows+numrows2)==0){
    	cout<<"You have no course to enroll."<<endl;
    	cout<<endl;
    	break;
	}
	cout<<"Courses you can select are listed as follows: "<<endl;
    cout<<"Course Code\t"<<"Quarter\t"<<"Year\t"<<"Enrollment\t"<<"Max\t"<<"Course Name\n";
    for(int i=0;i<numrows;i++){
        row=mysql_fetch_row(res_set);
//        row2=mysql_fetch_row(res_set2);
        if (row!=NULL) {
            cout<<row[0]<<"\t";
            cout<<row[2]<<"\t\t";
            cout<<row[3]<<"\t";
            cout<<row[4]<<"\t\t\t";
            cout<<row[5]<<"\t";
            cout<<row[1]<<"\n";
        }
    }
        for(int i=0;i<numrows2;i++){
            row2=mysql_fetch_row(res_set2);
            //        row2=mysql_fetch_row(res_set2);
            if (row2!=NULL) {
                cout<<row2[0]<<"\t";
                cout<<row2[2]<<"\t\t";
                cout<<row2[3]<<"\t";
                cout<<row2[4]<<"\t\t\t";
                cout<<row2[5]<<"\t";
                cout<<row2[1]<<"\n";
            }
        }
    string choice;

        cout<<"Please enter the code of the course you want to enroll:(Press 0 to go back to main menu) ";
        cin>>choice;
        if (choice.length()==1&&choice[0]=='0') {
            break;
        }
        cout<<"Please enter the quarter you want to take this course:(Press 0 to go back to main menu) ";
        string choice_quarter;
        cin>>choice_quarter;
        if (choice_quarter.length()==1&&choice_quarter[0]=='0') {
            break;
        }
        cout<<"Please enter the year you want to take this quarter:(Press 0 to go back to main menu)  ";
        string choice_year;
        cin>>choice_year;
        if (choice_year.length()==1&&choice_year[0]=='0') {
            break;
        }
        mysql_query(conn, query.c_str());
        res_set=mysql_store_result(conn);
        numrows=(int)mysql_num_rows(res_set);
        mysql_query(conn, query2.c_str());
        res_set2=mysql_store_result(conn);
        numrows2=(int)mysql_num_rows(res_set);
        bool flag=false;
        for(int i=0;i<numrows;i++){
            row=mysql_fetch_row(res_set);
            if (choice==row[0]&&choice_quarter==row[2]&&choice_year==row[3]) {
                flag=true;
                break;
            }
        }
        for(int i=0;i<numrows2;i++){
            row2=mysql_fetch_row(res_set2);
            if (choice==row2[0]&&choice_quarter==row2[2]&&choice_year==row2[3]) {
                flag=true;
                break;
            }
        }
        if (flag==false) {
            cout<<"Please enter a valid course code, quarter and year!"<<endl;
            continue;
        }
        cout<<"You want to enroll "<<choice<<endl;
        string enroll_query="CALL course_enroll("+username+", '"+choice+"', '"+choice_quarter+"', "+choice_year+");";
        cout<<"enroll_query: "<<enroll_query<<endl;
        mysql_query(conn, enroll_query.c_str());
        mysql_query(conn, "select @status;");
        res_set=mysql_store_result(conn);
        numrows=(int)mysql_num_rows(res_set);
        row=mysql_fetch_row(res_set);
        if (row[0]=="below"){
        	cout<<"The enrollment of this course is below 50%."<<endl;
		}
    }
}
void withdraw(string username){
	while(1){
    printf("***********Withdraw Menu***********\n");

    string  choice;
    time_t t = time(0);
    char year[5];
    char month[10];
    char month_num[3];
    string quarter;
    int num;
    
    
    strftime(year, sizeof(year), "%Y",localtime(&t));
    strftime(month, sizeof(month), "%B",localtime(&t));
    strftime(month_num, sizeof(month_num), "%m",localtime(&t));
    num = atoi(month_num);
    
    if (num>=1&&num<=3){
        quarter = "Q2";
    }
    else if(num>=4&&num<=6){
        quarter = "Q3";
    }
    else if(num>=7&&num<=9){
        quarter = "Q4";
    }
    else{
        quarter = "Q1";
    }
    cout<<"It's "<<month<<", "<<year<<", quarter: "<<quarter<<endl;
    
    cout<<endl;
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    string query="select transcript.UoSCode, unitofstudy.UoSName from transcript, unitofstudy where transcript.UoSCode=unitofstudy.UoSCode and StudId='"+username+"' and Year='"+year+"' and Semester='"+quarter
    +"' and Grade is null;";
    //cout<<"query: "<<query<<endl;
    mysql_query(conn, query.c_str());
    res_set=mysql_store_result(conn);
    int numrows=(int)mysql_num_rows(res_set);
    
    if (numrows==0){
    	cout<<"You have no course to withdraw."<<endl;
    	cout<<endl;
    	break;
	}
    cout<<"The courses you are currently enrolled in are as follows:"<<endl;
    for(int i=0;i<numrows;i++){
        row=mysql_fetch_row(res_set);
        if(row!=NULL){
            cout<<"UoSCode: "<<row[0]<<"\t";
            cout<<"UoSName: "<<row[1]<<endl;
        }
    }
    cout<<"Please enter the UoSCode you want to withdraw:(Press 0 to go back) ";
    

        cin>>choice;
        if(choice.length()==1&&choice[0]=='0') break;
        mysql_query(conn, query.c_str());
        res_set=mysql_store_result(conn);
        numrows=(int)mysql_num_rows(res_set);
        bool flag=false;
        for(int i=0;i<numrows;i++){
            row=mysql_fetch_row(res_set);
            if(choice==row[0]){
                flag=true;
                break;
            }
        }
        if (flag==false) {
            cout<<"Please enter a valid UoSCode!"<<endl;
            continue;
        }
        cout<<"You want to withdraw "<<choice<<endl;
        
        string withdraw="CALL course_withdraw("+username+", '"+choice+"', '"+quarter+"', "+year+");";
        //cout<<"withdraw: "<<withdraw<<endl;
        mysql_query(conn, withdraw.c_str());
        mysql_query(conn, "select @status;");
        res_set=mysql_store_result(conn);
        numrows=(int)mysql_num_rows(res_set);
        row=mysql_fetch_row(res_set);
        if (row[0]=="below"){
        	cout<<"The enrollment of this course is below 50%."<<endl;
        	cout<<endl;
        	
		}
        
    }
    
}
void personal_details(string username){
    while(1){
        printf("***********Personal Details***********\n");
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        string query="select ifnull(MaximumPreferredEnrollment,'Null'), ifnull(NonPreferredClassroomType,'Null') from student where Id="+username;
//        cout<<"query: "<<query<<endl;
        mysql_query(conn, query.c_str());
        res_set=mysql_store_result(conn);
        row=mysql_fetch_row(res_set);
        string maxnum=row[0];
        string nonprefertype=row[1];
        
        cout<<"Current MaximumPreferredEnrollment is: "<<maxnum<<endl;
        
        cout<<"Current NonPreferredClassroomType is: "<<nonprefertype<<endl;
        mysql_free_result( res_set );
        cout<<"1. Set MaximumPreferredEnrollment"<<endl;
        cout<<"2. Set NonPreferredClassroomType"<<endl;
        cout<<"0. Go Back"<<endl;
        string  choice;
        cin>>choice;
        if(choice.length()>1){
            cout<<"please enter valid choice!"<<endl;
            continue;
        }
        char c=choice[0];
        if(c=='0') break;
//        MYSQL_RES *res_set;
//        MYSQL_ROW row;

        else if(c=='1'){
            SetMaximumPreferredEnrollment(username);
        }
        else if(c=='2'){
            SetNonPreferredClassroomType(username);
        }
        else{
            cout<<"please enter valid choice!"<<endl;
        }
    }
}

void SetMaximumPreferredEnrollment(string username){
    cout<<"Please enter MaximumPreferredEnrollment(press 0 to go back): ";
    string maxnum;
    cin>>maxnum;
    
//    cout<<"string: "<<maxnum<<endl;
    string query="update student set MaximumPreferredEnrollment="+maxnum+ " where Id="+username+";";
//    cout<<query<<endl;
    mysql_query(conn, query.c_str());
}

void SetNonPreferredClassroomType(string username){
	while(1){
	
    cout<<"Please choose your NonPreferredClassroomType as below(press 0 to go back): "<<endl;
    
    MYSQL_RES *res_set;
	MYSQL_ROW row;
	string sql="select distinct Type from classroom;";
 
 
//	sql= sql + username;
//	cout<<sql<<endl;
	
	mysql_query(conn,sql.c_str());
	mysql_error(conn); 

            
    res_set = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(res_set);
	for(int i = 0;i<numrows;i++){
		row = mysql_fetch_row(res_set);
		if(row[0] != NULL){
			cout<<i+1<<"."<<row[0]<<endl;
			continue;
		}
	}
	
	 
    string j;
    string type;
    cin>>j;
    if(j.length()==1&&j[0]=='0') break;
    if(j.length()>1||j[0]<'1'||j[0]>(numrows+'0')){
    	cout<<"Please enter a valid choice!"<<endl;
    	continue;
	}
    mysql_query(conn,sql.c_str());
	mysql_error(conn); 

            
    res_set = mysql_store_result(conn);
	numrows = (int)mysql_num_rows(res_set);
	for(int i = 0;i<(j[0]-'0');i++){
		row = mysql_fetch_row(res_set);
		}
		type=row[0];
    
    
    

//    cin>>type;
    string query="update student set NonPreferredClassroomType='"+type+"' where Id="+username+";";
//    cout<<query<<endl;
	cout<<"Your NonPreferredClassroomType has been updated to *"+type+"*"<<endl;
    mysql_query(conn, query.c_str());
}
}



