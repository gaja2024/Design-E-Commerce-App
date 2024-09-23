#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric> // For std::accumulate
using namespace std;
// Struct to represent a product
struct Product {
    double price;
    double discount;   // Discount as a percentage (e.g., 0.2 for 20%)
    int quantitySold;
};

int main() {
    // Vector of products sold
    std::vector<Product> products = {
        {100.0, 0.10, 50},  // Product 1: Price = 100, 10% discount, 50 units sold
        {200.0, 0.15, 30},  // Product 2: Price = 200, 15% discount, 30 units sold
        {300.0, 0.20, 20},  // Product 3: Price = 300, 20% discount, 20 units sold
        {150.0, 0.05, 40}   // Product 4: Price = 150, 5% discount, 40 units sold
    };
    //store revnue 
    std::vector<double> revenues(products.size());
    // calculate total revenue
   transform(products.begin(),products.end(),revenues.begin(),[](const Product& product)
   {
        return (product.price-(product.price*product.discount))*product.quantitySold;
   });
   cout<<"Total revenue for each product"<<endl;
   // print each revenue
   for_each(revenues.begin(),revenues.end(),[](const double& revenue){
            cout <<revenue<<"\t";
   });
   cout<<endl;
   // calculate entire revenue for the store
   double total_revenue_entire_store = accumulate(revenues.begin(),revenues.end(),0 );
   cout<<"Total revenue for entire store : "<<total_revenue_entire_store<<endl;
}
  // double totalRevenue = accumulate(products.begin()->,products.end(),0)



