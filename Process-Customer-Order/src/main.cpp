// process-customer-order.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include<unordered_map>
#include<map>
#include <utility>
#include <iterator>
#include<algorithm>
#include<numeric>

using namespace std;
struct Order {
	int customerId;
	int orderId;
	std::vector<int> productIds;
};

std::vector<pair<int, double>> List_Customer_Expenditure_By_Descending(const std::vector<Order>& orders,
	const std::unordered_map<int, double>&productPriceMap)
{
	std::map<int, double> total_Expenditure_By_Customer;
	std::vector<pair<int, double>> find_total_Expenditure(orders.size());
	transform(orders.begin(), orders.end(), find_total_Expenditure.begin(), [&](const Order &order) {
		double nTotalCost = accumulate(order.productIds.begin(), order.productIds.end(), 0.0, [&](const double& nAccumulatedValue, const int& nCurElement) {
			return  nAccumulatedValue + productPriceMap.find(nCurElement)->second;
		});
		total_Expenditure_By_Customer[order.customerId] += nTotalCost;
		return std::make_pair(order.customerId, nTotalCost);
	});
	find_total_Expenditure.clear();
	find_total_Expenditure.resize(total_Expenditure_By_Customer.size());
	transform(total_Expenditure_By_Customer.begin(), total_Expenditure_By_Customer.end(), find_total_Expenditure.begin(),[](const pair<int, double>&mapElement) {
		return mapElement;
	});
	sort(find_total_Expenditure.begin(), find_total_Expenditure.end(), [](const pair<int, double>&element1, const pair<int, double>&element2) {
		return element1.second > element2.second;
	});
	return find_total_Expenditure;
}
void Remove_Duplicate_Orders(std::vector<Order>& orders)
{
	std::sort(orders.begin(), orders.end(), [](const Order & order1, const Order & order2) {
		return (order1.customerId < order2.customerId) ||
			((order1.customerId == order2.customerId) && (order1.orderId < order2.orderId)) ||
			((order1.customerId == order2.customerId) && (order1.orderId == order2.orderId) &&
				(order1.productIds < order2.productIds));
	});
	auto remove = std::unique(orders.begin(), orders.end(), [](const Order & order1, const Order & order2) {
		return (order1.customerId == order2.customerId) && (order1.orderId == order2.orderId) &&
			(order1.productIds == order2.productIds);

	});
	orders.resize(std::distance(orders.begin(),remove));
	
}
std::vector<int> Most_Frequently_Purchased_ProductID(const std::vector<Order>& orders)
{
	std::map<int, int>freq_purchased_productID;
	for_each(orders.begin(), orders.end(), [&](const Order &order) {
		transform(order.productIds.begin(), order.productIds.end(), std::inserter(freq_purchased_productID, freq_purchased_productID.end()), [&](const int &productID) {
			freq_purchased_productID[productID]++;
			return std::make_pair(productID, 1);
		});
	});
	auto maxElement = std::max_element(std::begin(freq_purchased_productID), std::end(freq_purchased_productID), 
		[](const auto &x, const auto &y) {
		return x.second < y.second;
	});
	std::vector<int>most_frequently_purchased_productID;
	for_each(freq_purchased_productID.begin(), freq_purchased_productID.end(), [&](const auto &mapElement) {
		if (mapElement.second == maxElement->second)
		{
			most_frequently_purchased_productID.push_back(mapElement.first);
		}
	});
	return most_frequently_purchased_productID;
}
std::vector<int> List_Of_VIP_Customers(const std::vector<pair<int, double>>&list_customer_expenditure_descending, double dThresholdAmount)
{
	vector<int>list_of_VIP_Customers;
	for_each(list_customer_expenditure_descending.begin(), list_customer_expenditure_descending.end(), [&](const pair<int, double>&element) {
		if (element.second > dThresholdAmount)
		{
			list_of_VIP_Customers.push_back(element.first);
		}
	});
	return list_of_VIP_Customers;
}
int main()
{
	std::unordered_map<int, double> productPriceMap = {
		{1, 100.0}, {2, 50.0}, {3, 75.0}, {4, 200.0}, {5, 150.0}
	};

	// Orders: (Customer ID, Order ID, List of Product IDs)
	std::vector<Order> orders = {
		{1, 1001, {1, 2, 3}},    // Customer 1 bought products 1, 2, 3
		{2, 1002, {2, 4}},       // Customer 2 bought products 2, 4
		{1, 1003, {1, 3, 5}},    // Customer 1 placed another order with products 1, 3, 5
		{3, 1004, {3}},          // Customer 3 bought product 3
		{2, 1005, {1,4}},          // Customer 2 placed another order with product 4
		//{1, 1001, {1, 4, 5}} ,    // Duplicate order by customer 1 (should be removed)
		{1, 1001, {1, 2, 3}}     // Duplicate order by customer 1 (should be removed)
	};
	//std::vector<Order> orders = {
	//	{3, 1001, {1, 2, 3}},    // Customer 3 bought products 1, 2, 3
	//	{2, 1002, {2, 4}},       // Customer 2 bought products 2, 4
	//	{3, 1003, {1, 3, 5}},    // Customer 3 placed another order with products 1, 3, 5
	//	{1, 1004, {3}},          // Customer 1 bought product 3
	//	{2, 1005, {4}} ,         // Customer 2 placed another order with product 4
	//	//{1, 1001, {1, 4, 5}} ,    // Duplicate order by customer 1 (should be removed)
	//	{3, 1001, {1, 2, 3}}     // Duplicate order by customer 3 (should be removed)
	//};
	Remove_Duplicate_Orders(orders);
	std::vector<pair<int, double>>list_customer_expenditure_descending = List_Customer_Expenditure_By_Descending(orders, productPriceMap);
	for_each(list_customer_expenditure_descending.begin(), list_customer_expenditure_descending.end(), [](const pair<int, double> & customer) {
		cout << "Customer " << customer.first << " has spent an amount of " << customer.second << endl;
	});
	std::vector<int>most_frequently_purchased_productID = Most_Frequently_Purchased_ProductID(orders);
	
	cout << "Most Frequent Product IDs are" << endl;
	for_each(most_frequently_purchased_productID.begin(), most_frequently_purchased_productID.end(), [](const int & productID) {
		cout << productID << "\t";
	});
	cout << endl;
	vector<int>list_of_VIP_Customers = List_Of_VIP_Customers(list_customer_expenditure_descending, 250.0);
	cout << "VIP Customers are" << endl;
	for_each(list_of_VIP_Customers.begin(), list_of_VIP_Customers.end(), [](const int & VIP) {
		cout << VIP << "\t";
	});
	return 0;
}


