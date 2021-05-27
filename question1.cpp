/*
                2020CSM1001

                AADITYA RAJ BARNWAL

*/
#include<bits/stdc++.h>
using namespace std;
#define infinite 60001                       // infinte value for sale_amount
#define REP(i,n) for(int i=0;i<n;i++)       // its used many times in program


int no_of_records,no_of_records_per_blocks,no_of_blocks_created,main_memory_size, files_counter,current_pass,intermediate_block;  // all are taken as global variable so that all can be accessed from every function and no need to pass an argument

// no_of_records = means total of records,records_per_block=size of the block,  main_memory_size=m blocks,file_counter is a variable used to count the number of runs, and current pass is for counting the no. of pass and also the same apply for intermediate_block


struct data{                    // created a structure
int t_id,s_amount;
string name;
int category;
};

void add_it(string line,vector<data> &a)    // it adds a string to the vector of data , here i am using it as  a reference so when vector chamges it reflect the same
{
    data element;
    vector <string> tokens;


    stringstream check1(line);

    string intermediate;


    while(getline(check1, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }
    element.t_id=stoi(tokens[0]);
    element.s_amount=stoi(tokens[1]);
    element.name=tokens[2];
    element.category=stoi(tokens[3]);
    a.push_back(element);

}

int calculate_sale_amount(string line)     // this function just calculate the sale amount of the given string, by breaking it into various string and its returning the sale amount
{

    vector <string> tokens;


    stringstream check1(line);

    string intermediate;


    while(getline(check1, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }

    return stoi(tokens[1]);


}



bool my_comparison_function(data d,data e)   // this function is used to sort the data, i am using this in sort function
{
    return d.s_amount<e.s_amount;

}
string convert_it_to_string(data d)        // it returns back the string whose fields are transaction_id,amount,name,category
{
           string enter=to_string(d.t_id);
            enter.push_back(' ');
            enter.append(to_string(d.s_amount));
             enter.push_back(' ');
             enter.append(d.name);
              enter.push_back(' ');
              enter.append(to_string(d.category));
            return enter;


}



string name_generate()                      // this function generate random 3 digit names
{
    char f='A'+rand()%26;
    char s='A'+rand()%26;
    char t='A'+rand()%26;
    string name="";
    name.push_back(f);
    name.push_back(s);
    name.push_back(t);
    return name;

}


void create_dataset()                       // this function create_dataset
{
    srand(time(0));
    int records;
    cout<<"Enter the No. of Records \n";    // asking the user to enter the total number of records
    cin>>no_of_records;
    records=no_of_records;
    string filename="dataset.txt";
    fstream new_file;
    new_file.open(filename,ios::out);    // creating a file dataset.txt , which contains the all data
    if(!new_file)
    {
        cout<<"unable to create the file";
        return ;
    }
    REP(i,records)
    {
        string line="";
        line.append(to_string(i+1));
        line.push_back(' ');

        int sale_amount=(rand()*rand()*1ll)%60000 + 1;
        line.append(to_string(sale_amount));
        string name=name_generate();
        line.push_back(' ');
        line.append(name);
        int category=rand()%1500+1;
        line.push_back(' ');
        line.append(to_string(category));
        new_file<<line;
        new_file<<"\n";


    }

}



void create_disk_blocks()           // this function works after you have created the dataset a, basically it divides all the records into various blocks
{
   int b=40;
   int counter=0;
   cout<<"Enter the No. of records in per Block that is the value of B\n";
   cin>>no_of_records_per_blocks;
   b=no_of_records_per_blocks;


   string path="blocks";
   string filename=path;
   filename.append(to_string(counter));
   filename.append(".txt");
   fstream new_file,read_from_dataset;
   new_file.open(filename,ios::out);
   read_from_dataset.open("dataset.txt",ios::in);
   string tp;
   for(int i=1;i<no_of_records+1 && getline(read_from_dataset, tp);i++)
   {

       if((i-1)%b==0 && i!=1){
       counter++;
       new_file.close();
       filename=path;
      filename.append(to_string(counter));
      filename.append(".txt");

       new_file.open(filename,ios::out);
       }

       new_file<<tp<<endl;


   }

   no_of_blocks_created=counter;


   new_file.close();
}



void simple_sort_it_and_write_it(){   // this function is called  when memory is larger than total no of blocks and in this case evrything can be stored in memory and simple sorting and printing the output
    vector<data> a;


    REP(i,no_of_blocks_created+1)
    {
        string name="blocks",tp;;
        name.append(to_string(i));
        name.append(".txt");
        fstream file;
        file.open(name,ios::in);
        while(getline(file,tp))
            add_it(tp,a);
        file.close();

    }
    sort(a.begin(),a.end(),my_comparison_function);
    fstream file;
    file.open("output.txt",ios::out);
    REP(i,a.size())
    {
        file<<convert_it_to_string(a[i])<<endl;

    }






}





bool check_every_files_has_completed_or_not(int a[],int n)     // this function is used in merge_files_ so that to check whether the all the files are merged or not
{
    REP(i,n)
    if(a[i]!=infinite)return true;
    return false;

}

string assign_max_value(){   // this function is used to assign max value to sale_amount, the same way we used to do in merge_sort and it return a string

string s="";s.append(to_string(188));// just random values;
s.push_back(' ');
s.append(to_string(infinite));

s.push_back(' ');
s.append(name_generate());
s.push_back(' ');
s.append(to_string(1));
return s;

}




void merge_blocks(vector<string> names) // this is the heart of this program
/*

            this function taken a vector of names , of merge these files into a single file

*/
{
    if(names.size()==1) // it means that I have to merge only 1 block , in other words it means that the same block will be carry on to the next pass, so I doing the same
    {


          fstream out_files,read_files;
          read_files.open(names[0],ios::in);
           string new_name="pass ";                 // reading the file first and writing the same content in a new file as there is only 1 file
           new_name.append(to_string(current_pass));
           new_name.push_back(' ');
           new_name.append("runs");
           new_name.append(to_string(files_counter));
            files_counter++;
           new_name.append(".txt");
           out_files.open(new_name,ios::out);
           while(getline(read_files,new_name))
            out_files<<new_name<<endl;



    }
    else    // this means that names size is greater than 1
    {
        int n=names.size();
        fstream files[n],out_files;  // creating the array of file pointer,and out_files is for output which we get after merging all of this
        string current[n];
        string new_name="pass ";
           new_name.append(to_string(current_pass));
           new_name.push_back(' ');
           new_name.append("runs");
           new_name.append(to_string(files_counter));
            files_counter++;
           new_name.append(".txt");  // upto this point I am just renaming the output file, in a particular manner so that it can be accessed in the next iteration
           out_files.open(new_name,ios::out);
        REP(i,n){
        files[i].open(names[i],ios::in);
        getline(files[i],current[i]);    // just opening each file and reading the fisrt line
        }
        bool falg=true;    // just like a flag
        while(falg)
        {
            int current_sale_amount[n];    // array of sale_amount,as we have to sort it based on sale_amount
            REP(i,n)
            current_sale_amount[i]=calculate_sale_amount(current[i]);   // this is function that i have defined in the above pls refer it,it returns the sale amount
            falg=check_every_files_has_completed_or_not(current_sale_amount, n);  // condition of checking whether all files data has read or not
            if(!falg)
            break;   // if all files data has read, has just break
            int minimum=current_sale_amount[0];   // here i am finding out the minimum value of sale_amount, by just classical method
            int index=0;        //it points the index of minimum value
            REP(i,n)
            {
                if(minimum>current_sale_amount[i])
                {
                    minimum=current_sale_amount[i];
                    index=i;
                }

            }
            out_files<<current[index]<<endl;   // writing this minimum value in output file



            if(getline(files[index],current[index]))   // if end of file is not reached then, it will read successfully into current[index],else
            {
            }else
                current[index]=assign_max_value();// here i am assigning max value,  as mentioned above , its the same procedure of merge_sort algorithm




        }


REP(i,n)files[i].close();
out_files.close();  // closing everything


    }
//  after this point i am creating blocks by blocks in each pass ie intermediate blocks in each pass
string new_name="pass ";
           new_name.append(to_string(current_pass));
           new_name.push_back(' ');
           new_name.append("runs");

           new_name.append(to_string(files_counter-1));
                                                            // naming convention as explained in readme file
           new_name.append(".txt");

    fstream file;string line;
    file.open(new_name,ios::in);
    string name="pass ";
    name.append(to_string(current_pass));
    name.push_back(' ');
    name.append("blocks");
    name.append(to_string(intermediate_block));
    intermediate_block++;
    name.append(".txt");
    fstream out;
    out.open(name,ios::out);
    int size_blck=0;     // for knowing how many records have been put up in each file, if it exceeds then store it next block
    while(getline(file,line))
    {

    out<<line<<endl;
    size_blck++;

    if(size_blck==no_of_records_per_blocks && getline(file,line))   // checking whether 1 block records created or not, here i am also added getline function because it you dont add this and sometimes it may be possible that it may create empty files
    {
       out.close();
        name="pass ";     // naming convention
    name.append(to_string(current_pass));
    name.push_back(' ');
    name.append("blocks");
    name.append(to_string(intermediate_block));
    intermediate_block++;
    name.append(".txt");
    out.open(name,ios::out);
    out<<line<<endl;  // writing everything as describe in readme file
    size_blck=1;
    }





    }



}


void sort_each_block_and_store()   // this is the first step, here i am sorting each block individually,in itself
{
    string path="blocks";
    string pass="pass 1blocks ";   // i am keeping the name convention as mentioned in the readme file
    REP(i,no_of_blocks_created+1)
    {
        string name=path;
        vector<data> a;
        name.append(to_string(i));
        name.append(".txt");
        fstream file,file2,file3;
        file.open(name,ios::in);
        string tp;
        while(getline(file,tp))
            add_it(tp,a);
        file.close();
        sort(a.begin(),a.end(),my_comparison_function);
        string path2="pass 1 runs";
        path2.append(to_string(i));
        string pass2=pass;
        pass2.append(to_string(i));
        path2.append(".txt");
        pass2.append(".txt");
        file2.open(path2,ios::out);
        file3.open(pass2,ios::out);
        REP(i,a.size())
        {file2<<convert_it_to_string(a[i])<<endl;
        file3<<convert_it_to_string(a[i])<<endl;}

    }


}

int my_ceil_function(int num,int den)  // ceil function
{
    int ans=num/den;
    int r=0;
    if(num%den!=0)r=1;
    return r+ans;

}
int minimum_calculate(int a,int b) // calculate minimum value
{
    return a>b?b:a;

}

void now_do_everything_and_sort_it()   // its the function which iteratively going to again and again untill everything got sorted
{


int m;
cout<<"Whats the size of memory in blocks that is the value of M \n";  // asking the user the memory size
cin>>m;
main_memory_size=m;

if(m>no_of_blocks_created+1)   // this condtion is when main memory size is greater than no. of blocks , then sort everything
{
    simple_sort_it_and_write_it();
    return;
}
sort_each_block_and_store();
int now=no_of_blocks_created+1;  // current no. of block

int prev_pass=1;  // for reading purpose
while(1)
{
   if(now==1)break;   // break condtion,means everything got sorted
    int times=my_ceil_function(now,m-1);    // calculating the number of runs in each pass
    int track=0;                                //for naming convention

    files_counter=0;            // global variable for naming convention
    current_pass=prev_pass+1;
    intermediate_block=0;       // for naming convention
    REP(i,times)
    {
        int how_many_blocks_at_a_time=minimum_calculate(now,m-1);   // minimum of blocks left, memory size
        vector<string > a;

        REP(j,how_many_blocks_at_a_time)        // in this loop i am getting all the file name that have to merge
        {
            string path="pass ";
            path.append(to_string(prev_pass));
            path.push_back(' ');
            path.append("runs");
            path.append(to_string(track));
            path.append(".txt");
            track++;
            a.push_back(path);

        }

       now=now-m+1;  // updating no of file left

        merge_blocks(a);  // calling function to merge it the files
        a.clear();  //   clearing the vector of strings, its size is always 'm-1'


    }
    prev_pass++;   // updating prev pass value
    now=times;      // updating the no of blocks currently created and storing it in now






}

// now here everything is sorted now i am just creating a file output.txt , which contains the result that is sorted files
string output_name="output.txt";
fstream out,input;
out.open(output_name,ios::out);
string reading_from="pass ";
reading_from.append(to_string(current_pass));
reading_from.push_back(' ');
reading_from.append("runs0.txt");
input.open(reading_from,ios::in);
while(getline(input,output_name))
    out<<output_name<<endl;






}




int main(){

create_dataset();// creating data set
cout<<"Data sets Created \n";
create_disk_blocks(); // dividing into blocks
cout<<"Everything divided into blocks \n";

now_do_everything_and_sort_it(); // this function do everything



return 0;
}



/*
In this program , what i am actually doing is
step by step:

1. I am creating data set

2. I am dividing it into blocks

3.Now I am checking the memory size , whether its greater or lesser than no. of blocks,if it is greater then no need to worry just store everything in memory and sort it, else
    I am dividing the blocks in m-1 partitions, and merging them, and considering the naming convention and store it, naming convention is important because we are doing this iteratively
    ,so we dont know after how many iteration it will stop, so naming convention is very important , so that we can use the prev blocks to generate new blocks

4. we are repeating step 3 until all the files is not merged
5. finally we are created a file name as output.txt which contains the desired result





*/


