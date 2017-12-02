#include "grid.h"
#include <iostream>
#include <time.h>

using namespace std;

int main()
{
  clock_t t;
  t = clock();
    grid G("../icon_grid_0026_R03B07_G.nc");
  t = clock() - t;
  cout << "init time: " << ((double)t)/CLOCKS_PER_SEC << '\n';

  std::string input;
  double lon, lat;

  // How to get a string/sentence with spaces
  cout << "Please enter a longitude:\n>";
  getline(cin, input);
  lon = std::stod(input);
  cout << "You entered: " << lon  << endl << endl;

  cout << "Please enter a latitude:\n>";
  getline(cin, input);
  lat = std::stod(input);
  cout << "You entered: " << lat << endl << endl;

  t = clock();
    auto e = G.XY(point(lon,lat));
  t = clock() - t;

  cout << "time: " << ((double)t)/CLOCKS_PER_SEC << '\n';

  cout << "index " << e->index << " center " << e->center_point << '\n';
  for(auto& x : e->vertices)
      cout << x;
  
  return 0;
}
