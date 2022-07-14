#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;
void mySleep(int ms) // A cross platform function that halts the program for specified amt of time
{
#ifdef WIN32
  Sleep(ms);
#elif _POSIX_C_SOURCE >= 199309L
  struct timespec t;
  t.tv_sec = ms / 1000;
  t.tv_nsec = (ms % 1000) * 1000000;
  nanosleep(&t, NULL);
#else
  usleep(ms * 1000);
#endif
}

int get_distance(int x1, int y1, int x2, int y2) //gives |x2-x1|+|y2-y1|
{
  if (x1 >= x2 && y1 >= y2)
  {
    return x1 - x2 + y1 - y2;
  }
  else if (x1 >= x2 && y2 >= y1)
  {
    return x1 - x2 + y2 - y1;
  }
  else if (x2 >= x1 && y1 >= y2)
  {
    return x2 - x1 + y1 - y2;
  }
  else
  {
    // x2>x1 && y2>y1
    return x2 - x1 + y2 - y1;
  }
}
class Person
{
public:
  int x, y, d_x, d_y;                    //x,y are position coordinates; d_x,d_y are destination coordinates
  Person(int x, int y, int d_x, int d_y) //Constructor for Person
  {
    this->x = x;
    this->d_x = d_x;
    this->y = y;
    this->d_y = d_y;
  }

  void ride() //Updates the Person's coordinates to their destination coordinates after they
              // finish their ride
  {
    this->x = this->d_x;
    this->y = this->d_y;
  }
};

class Cars
{
private:
  int base_cost, cost_per_km, base_distance;
  // base_cost -- is the cost before base_distance amt of km

public:
  int x, y, total_cost, license; // x,y are x,y coordinates of the car
  float total_rides, rating;     //total_rides is total number of rides of the car so far
                                 //rating is the rating (out of 5) of the car so far

  string name, type;
  Cars(string n, int x, int y, int l, float r, int t)
  { //Constructor that assigns all fields, including type of the car based on name.
    this->x = x;
    this->y = y;
    this->license = l;
    this->rating = r;
    this->total_rides = t;
    name = n;

    if (n == "Dzire" || n == "Amaze" || n == "Vento" || n == "City" ||
        n == "Verna" || n == "Rapid" || n == "Ciaz")
    {
      this->type = "Sedan";
      this->base_distance = 7;
      this->base_cost = 100;
      this->cost_per_km = 15;
    }
    else if (n == "i20" || n == "i10" || n == "Swift" || n == "Baleno" ||
             n == "Kwid" || n == "WagonR" || n == "Polo")
    {
      this->type = "Hatchback";
      this->base_distance = 8;
      this->base_cost = 80;
      this->cost_per_km = 12;
    }
    else if (n == "Innova" || n == "Brezza" || n == "Seltos" ||
             n == "Sonet" || n == "Creta" || n == "Ertiga" ||
             n == "Fortuner")
    {
      this->type = "SUV";
      this->base_distance = 8;
      this->base_cost = 120;
      this->cost_per_km = 17;
    }
    else if (n == "BMW" || n == "Audi" || n == "Mercedes" ||
             n == "LandRover" || n == "Jaguar" || n == "Porsche" ||
             n == "Lexus")
    {
      this->type = "Luxury";
      this->base_distance = 12;
      this->base_cost = 200;
      this->cost_per_km = 20;
    }
  }

  int get_cost(Person p) //gives cost of the taxi ride of this car for Person p
  {
    int d = this->get_distance_from_person(p) +   //Total ditance travelled by car =
            get_distance(p.x, p.y, p.d_x, p.d_y); // distance from person + distance from person to destination
    if (d < base_distance)
    {
      return this->base_cost;
    }
    else
    {
      return ((d - base_distance) * this->cost_per_km + this->base_cost);
    }
  }

  int get_distance_from_person(Person p) // gives cost of this car from Person p
  {
    return get_distance(p.x, p.y, x, y);
  }

  void print_data(Person p)
  {
    std::cout << "Car: " << name << "\tLicense:" << license << "\tLocated at (" << x
              << "," << y
              << ")\tDistance from you: " << this->get_distance_from_person(p)
              << endl;
  }
  void rate() //Allows user to rate their ride and updates the rating of the car as well as total rides
  {
    std::cout << "Rate your ride from 1-5!";
    int r;
    cin >> r;
    float total_sum = this->rating * this->total_rides + r;
    this->total_rides = this->total_rides + 1;
    this->rating = (total_sum) / (total_rides);
  }
  int driver_rate() //This is for when user has logged in as a taxi driver
  {
    //Provides a random rating to the driver and updates the rating
    srand(time(0));
    int r = 1 + (rand() % (5));
    float total_sum = this->rating * this->total_rides + r;
    this->total_rides = this->total_rides + 1;
    this->rating = (total_sum) / (total_rides);
    return r;
  }
}; //Class Cars ends
void in_data(vector<Cars> &v, ifstream &in)
{ //reads data from input file in and stores it in vector v
  for (int i = 0; i < 10; i++)
  {
    string name;
    int x, y, l, t;
    float r;
    in >> name >> x >> y >> l >> r >> t;
    v.push_back(Cars(name, x, y, l, r, t));
  }
}
void out_data(vector<Cars> v, ofstream &out)
{ //reads data from vector v and writes it into out
  for (int i = 0; i < 10; i++)
  {
    Cars c = v.at(i);
    out << c.name << " " << c.x << " " << c.y << " " << c.license << " " << c.rating << " " << c.total_rides << endl;
  }
}

int choose_and_ride(vector<Cars> &v, Person p) //provides the closest car from v to Person p and returns the cost
{
  int least = v.at(0).get_distance_from_person(p);
  int x = 0;
  int f = 0;
  std::cout << "Cars available in your chosen category are: " << endl;

  for (auto car : v)
  {
    if (car.get_distance_from_person(p) < least) //if v[i] car is closer, update least distance to distance from v[i]
    {
      least = car.get_distance_from_person(p);
      f = x;
    }
    x++;
    car.print_data(p); //prints all available cars
  }

  std::cout << "\nThe closest car to you has been chosen, which is: " << endl;
  v.at(f).print_data(p);
  std::cout << "\nTotal cost of journey is Rs." << v.at(f).get_cost(p) << endl;
  std::cout << "\nDriver will reach you in " << v.at(f).get_distance_from_person(p)
            << " minutes. Enjoy your ride!\n"
            << endl;
  int cost1 = v[f].get_cost(p);
  p.ride();
  mySleep(500);
  std::cout << "Your ride is finished! You are now at " << p.x << ","
            << p.y << endl;
  v.at(f).rate();
  return cost1;
}
int main()
{
  vector<Cars> sedans;
  vector<Cars> hatchbacks;
  vector<Cars> SUVs;
  vector<Cars> Luxury;

  ifstream in_sedans, in_hatchbacks, in_SUVs, in_Luxury;
  //open all the 4 text files
  in_sedans.open("sedans.txt");
  in_hatchbacks.open("hatchbacks.txt");
  in_SUVs.open("SUVs.txt");
  in_Luxury.open("Luxury.txt");
  //read data from all text files into their respective vectors
  in_data(sedans, in_sedans);
  in_data(hatchbacks, in_hatchbacks);
  in_data(SUVs, in_SUVs);
  in_data(Luxury, in_Luxury);
  //close the input streams
  in_sedans.close();
  in_hatchbacks.close();
  in_SUVs.close();
  in_Luxury.close();
  cout << "Welcome to DTU CABS, a state of art cab booking program designed by first year computer engineering students  " << endl;

  cout << "Do you want to login as a customer(Y/N)?" << endl;
  char inp_mode;
  cin >> inp_mode;
  if (inp_mode == 'Y')
  { //LOGGED IN AS CUSTOMER
    int run = 0, ride_cost = 0;
    Person p = Person(0, 0, 0, 0); //initialise person at 0,0
    string f_name, l_name;
    int p_x, p_y;

    while (true)

    {
      if (run == 0) //first time riding in taxi
      {
        std::cout << "Welcome! Enter your full name. (Type exit program to exit)" << endl;

        std::cin >> f_name >> l_name;
        if (f_name == "exit" && l_name == "program")
        {
          return 0;
        }
        std::cout << "Welcome " << f_name << " " << l_name
                  << "! Enter your coordinates, separated by a space." << endl;

        std::cin >> p_x >> p_y;
        //update person coordinates from 0,0 to coordinates user entered
        p.x = p_x;
        p.y = p_y;
      }
      else
      {
        std::cout << "Would you like to ride again?[Y/N]" << endl;
        char x;
        std::cin >> x;
        if (x == 'N')
        {
          break;
        }
      }
      run = 1; //Run is updated to 1 as user will have already rode once now

      std::cout << "Enter your destination coordinates, separated by a space." << endl;
      int d_x, d_y;
      std::cin >> d_x >> d_y;
      //Update Person destination coordinates to user entered destination coordinates
      p.d_x = d_x;
      p.d_y = d_y;
      bool flag = true; //flag is there as a check to see if the user input is valid or not
      while (flag)
      {
        std::cout << "Choose your category :sedan,SUV,hatchback or luxury" << endl;
        cout << "Pricing model" << endl;
        cout << "1. Sedan      base distance = 7 KM    base cost = Rs. 100    Cost per km = Rs.15 " << endl;
        cout << "2. SUV        base distance = 8 KM    base cost = Rs. 120    Cost per km = Rs.17 " << endl;
        cout << "3. Hatchback  base distance = 8 KM    base cost = Rs. 80     Cost per km = Rs.12 " << endl;
        cout << "4. Luxury     base distance = 12 KM   base cost = Rs.200     Cost per km = Rs.20 " << endl;
        string category;
        std::cin >> category;
        //call choose and ride function according to chosen category and store the cost of ride
        if (category == "sedan")
        {
          ride_cost = choose_and_ride(sedans, p);
          flag = false;
        }
        else if (category == "hatchback")
        {
          ride_cost = choose_and_ride(hatchbacks, p);
          flag = false;
        }
        else if (category == "SUV")
        {
          ride_cost = choose_and_ride(SUVs, p);
          flag = false;
        }
        else if (category == "luxury")
        {
          ride_cost = choose_and_ride(Luxury, p);
          flag = false;
        }
        else
        {
          std::cout << "Oops! Try again! " << endl;
        }
      }
    }

    int quiz_discount = 0;
    cout << "Do you want to participate in a short quiz to get discount :(Y/N) " << endl;
    char inp;
    cin >> inp;
    while (true)
    {
      if (inp == 'Y' || inp == 'N')
      {
        break;
      }
      else
      {
        cout << "Invalid Input,enter again!" << endl;
        cin >> inp;
      }
    }
    if (inp == 'Y')
    { //Simple Quiz Game
      cout << "The quiz consists of 5 questions, every correct answer will give you a discount of 10 rs and incorrect answer won't affect your fare" << endl;
      cout << "Q1. what is 2+2?" << endl;
      int x = 0;
      cin >> x;
      if (x == 4)
      {
        cout << "Correct answer,well done!" << endl;
        quiz_discount += 10;
      }
      else
      {
        cout << "Incorrect answer,better luck next time!" << endl;
      }
      cout << "Q2. what is 3*4?" << endl;
      x = 0;
      cin >> x;
      if (x == 12)
      {
        cout << "Correct answer,well done!" << endl;
        quiz_discount += 10;
      }
      else
      {
        cout << "Incorrect answer,better luck next time!" << endl;
      }

      cout << "Q3. what is 12/2?" << endl;
      x = 0;
      cin >> x;
      if (x == 6)
      {
        cout << "Correct answer,well done!" << endl;
        quiz_discount += 10;
      }
      else
      {
        cout << "Incorrect answer,better luck next time!" << endl;
      }
      cout << "Q4. what is 19*12-12*19" << endl;
      x = 0;
      cin >> x;
      if (x == 0)
      {
        cout << "Correct answer,well done!" << endl;
        quiz_discount += 10;
      }
      else
      {
        cout << "Incorrect answer,better luck next time!" << endl;
      }
      cout << "Q5. what is 5-5+5-5+5-5+5" << endl;
      x = 0;
      cin >> x;
      if (x == 5)
      {
        cout << "Correct answer,well done!" << endl;
        quiz_discount += 10;
      }
      else
      {
        cout << "Incorrect answer,better luck next time!" << endl;
      }
      cout << "Congratulations!,your reward is a discount of " << quiz_discount << endl;
      cout << "Effective Fair is  " << (ride_cost - quiz_discount) << endl;
    }

    std::cout << "Thanks for riding! Bye!" << endl;
  } //Customer Login Ends
  else
  { //LOGGED IN AS TAXI DRIVER
    Cars driver_car = sedans[0];
    int total_money = 0; // total money earned
    cout << "enter your category?(sedans/hatchbacks/SUVs/Luxury)" << endl;
    string cat_driver;
    cin >> cat_driver;
    int key = 0;
    cout << "Enter your login key" << endl; //each driver has a unique login key, which is equal to their car's index in their respective vectors
    cin >> key;
    cout << "Enter your License No" << endl;
    int lic_check;
    cin >> lic_check;
    //for each driver, login key serves as username whereas their license number serves as password
    //check to see if username and password match:
    if (cat_driver == "sedans")
    {
      if (lic_check == sedans[key].license)
      {
        driver_car = sedans[key]; //Assign driver_car as the car that user logged in as
        cout << "Sucessful Login as a driver...." << endl;
        cout << "welcome " << sedans[key].name << "!" << endl;
        cout << "Your rating is " << sedans[key].rating << "." << endl;
      }
      else
      {
        cout << "ERROR. WRONG DETAILS.";
        return 1;
      }
    }
    //similar code for other categories:
    else if (cat_driver == "hatchbacks")
    {
      if (lic_check == hatchbacks[key].license)
      {
        driver_car = hatchbacks[key];
        cout << "Sucessful Login as a driver...." << endl;
        cout << "welcome " << hatchbacks[key].name << "!" << endl;
        cout << "Your rating is " << hatchbacks[key].rating << "." << endl;
      }
      else
      {
        cout << "ERROR. WRONG DETAILS.";
        return 1;
      }
    }
    else if (cat_driver == "SUVs")
    {
      if (lic_check == SUVs[key].license)
      {
        driver_car = SUVs[key];
        cout << "Sucessful Login as a driver...." << endl;
        cout << "welcome " << SUVs[key].name << "!" << endl;
        cout << "Your rating is " << SUVs[key].rating << "." << endl;
      }
      else
      {
        cout << "ERROR. WRONG DETAILS.";
        return 1;
      }
    }
    else if (cat_driver == "Luxury")
    {
      if (lic_check == Luxury[key].license)
      {
        driver_car = Luxury[key];
        cout << "Sucessful Login as a driver...." << endl;
        cout << "welcome " << Luxury[key].name << "!" << endl;
        cout << "Your rating is " << Luxury[key].rating << "." << endl;
      }
      else
      {
        cout << "ERROR. WRONG DETAILS.";
        return 1;
      }
    }
    while (true)
    {                                                  // infinite loop that runs till user says they want to end their day
      cout << "Would you like to end your day? [Y/N]"; //asked after each ride
      char c;
      cin >> c;
      if (c == 'Y')
      {
        cout << "You earned " << total_money << " today. Goodbye!" << endl;
        break;
      }
      else
      {
        int x, y, dx, dy;
        //Assign passenger with random coordinates to Driver
        srand(time(0));
        x = rand() % 50;
        dx = rand() % 50;
        y = rand() % 50;
        dy = rand() % 50;
        //Create a Person object
        Person p(x, y, dx, dy);
        cout << "Your passenger is at " << x << "," << y
             << " and their destination is " << dx << " " << dy << endl
             << "Go to your passenger and drive them to their destination." << endl;
        mySleep(1000);
        cout << "Your ride is over. You earned Rs." << driver_car.get_cost(p) << "!" << endl;
        int rat;

        total_money += driver_car.get_cost(p); //Add money earned to total_money
        if (cat_driver == "sedans")
        {
          rat = sedans[key].driver_rate(); //updates car rating and assigns rat as whatever value they were rated
        }
        else if (cat_driver == "hatchbacks")
        {
          rat = hatchbacks[key].driver_rate();
        }
        else if (cat_driver == "SUVs")
        {
          rat = SUVs[key].driver_rate();
        }
        else
        {
          rat = SUVs[key].driver_rate();
        }
        cout << "You were rated " << rat << endl; //shows the driver what they were rated
      }
    }
  }
  ofstream out_hatchbacks, out_sedans, out_Luxury, out_SUVs;
  out_hatchbacks.open("hatchbacks.txt");
  out_SUVs.open("SUVs.txt");
  out_Luxury.open("Luxury.txt");

  //output the updated data of all the cars into their respective txt fies:
  out_sedans.open("sedans.txt");
  out_data(sedans, out_sedans);
  out_data(hatchbacks, out_hatchbacks);
  out_data(SUVs, out_SUVs);
  out_data(Luxury, out_Luxury);
  //close all the output streams:
  out_sedans.close();
  out_hatchbacks.close();
  out_SUVs.close();
  out_Luxury.close();
}
