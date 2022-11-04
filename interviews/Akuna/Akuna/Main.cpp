#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

// operation strings
const string BUY_STR = "BUY";
const string SELL_STR = "SELL";
const string CANCEL_STR = "CANCEL";
const string MODIFY_STR = "MODIFY";
const string PRINT_STR = "PRINT";

// order type strings
const string IOC_STR = "IOC";
const string GFD_STR = "GFD";

constexpr char DELIM = ' ';

enum operation_type
{
    BUY,
    SELL,
    CANCEL,
    MODIFY,
    PRINT,
    UNSET
};

enum order_type
{
    IOC,    // Insert or Cancel- if can't be immediately traded, cancelled.
            // If only partially traded, non-traded part is canceled.
    GFD     // Good For Day- Stays in order book until completely traded
};

struct Order
{
    order_type type;
    int price;
    int quantity;
    time_t timeStamp;

    Order() : type(GFD), price(0), quantity(0), timeStamp(0)
    {
        // do nothing
    }

    Order(const order_type& type, const int price, const int quantity, const time_t timeStamp) :
        type(type), price(price), quantity(quantity), timeStamp(timeStamp)
    {
        // do nothing
    }
};

struct PriceQuantityID
{
    int price;
    int quantity;
    string orderID;

    PriceQuantityID() : price(0), quantity(0), orderID("")
    {
        // do nothing
    }

    PriceQuantityID(const int price, const int quantity, const string orderID) :
        price(price), quantity(quantity), orderID(orderID)
    {
        // do nothing
    }
};

struct Modification
{
    operation_type operation; // BUY or SELL
    int price;
    int quantity;
    
    Modification(const operation_type& operation, const int price, const int quantity) :
        operation(operation), price(price), quantity(quantity)
    {
        // do nothing
    }
};

class MatchingEngine
{
    // Matching Rule:
    // Price Cross- someone is willing to buy at a price >= lowest current selling price, orders are
    // considered trading against each other.
    // If this is encountered, the trade info should be printed as follows:
    // BUY GFD 1000 10 order1
    // SELL GFD 900 10 order2
    // ==> TRADE order1 1000 10 order2 900 10
    // NOTE: may involve multiple buyers / sellers, so each should be printed.
    // TODO: after each BUY or SELL order, need to check for prices cross event.
    // NOTE: the sequence is decided by which was processed first.
    // TODO: need to update the state of m_priceBook when a trade occurs?
    // NOTE: Use price-time priority. Orders equally priced are traded out and in the order they are received.
    // TODO: how to keep track of this? Use a queue? Timestamp?
    // NOTE: if an order is modified, loses original priority in the queue. 
    // NOTE: OrderID should be unique for all active orders in the matching engine, otherwise the operation
    // should be ignored
    // QUESTION: should the TRADE automatically print, or only if given a 'PRINT' command? Assume yes...
    // (see Example 4)
    public:
    void process(const Order& order)
    {
        
    }
    
    private:
    queue<Order> m_buys;
    queue<Order> m_sells;
    
    void print()
    {
        // If trade encountered, print orders involved
        
    }
};

class OrderManager
{
    public:
    OrderManager() : m_buysMap(), m_sellsMap()
    {
        // do nothing
    }
    
    // Use cases:
    // * Buy- Store the order info.
    // * Sell- Store the order info.
    // * Cancel- Order ID provided. If found, delete the order, else ignore.
    // * Modify- Check order exists. Update operation, price, quantity. NOTE: can't modify IOC type! 
    // * Print- If other columns, just ignore. Print current order map (m_priceBook). Split into 'SELL'
    // and 'BUY' sections, each in decreasing order based on price (Need to do sort operation). Sum
    // all orders with the same price
    
    // WARNING: need close coupling between m_priceBook and m_buys/m_sells... (Example 4)
    
    // QUESTION: Example 5- is the TRADE output correct? Seems ORDER1 was processed first, so shouldn't it
    // be traded first??
    void buy(const string& orderID, const Order& order)
    {
        // TODO: if order.type == IOC, only process if can be traded immediately. If only partially traded,
        // remainder should be canceled
        // ignore non-unique order IDs

        const int updatedBuyQty = checkPriceCrossOnBuy({order.price, order.quantity, orderID});
        Order updatedOrder(order.type, order.price, updatedBuyQty, order.timeStamp);


        if (shouldRecord(orderID, order)) {
			// get the current time stamp
			const time_t timeStamp = time(0);

            m_buysMap[orderID] = order;       
            m_buysPriceOrderedMap[order.price] = { order.quantity, orderID };
            m_buysTimeOrderedMap[timeStamp] = { order.price, order.quantity, orderID };
        }
    }
    
    void sell(const string& orderID, const Order& order)
    {
        // TODO: if order.type == IOC, only process if can be traded immediately. If only partially traded,
        // remainder should be canceled
        // ignore non-unique order IDs
        if (shouldRecord(orderID, order)) {
            m_sellsMap[orderID] = order;       
        }
    }
    
    void cancel(const string& orderID)
    {
        // erase the entry with orderID key if it exists
        // TODO: update all data structures / maps here
        time_t timeStamp = m_buysMap[orderID].timeStamp;
        m_buysMap.erase(orderID);
        m_buysTimeOrderedMap.erase(timeStamp);

        time_t timeStamp = m_sellsMap[orderID].timeStamp;
        m_sellsMap.erase(orderID);
        m_sellsTimeOrderedMap.erase(timeStamp);
    }
    
    // * Modify- Check order exists. Update operation, price, quantity. NOTE: can't modify IOC type! 
    void modify(const string& orderID, const Modification& mod)
    {
        if (canModify(orderID, mod)) {
            operation_type modOperation = mod.operation;
            // TODO: update all data structures / maps here
            if (modOperation == BUY && m_buysMap.count(orderID) > 0) {
                Order& orderRef = m_buysMap[orderID];
                orderRef.price = mod.price;
                orderRef.quantity = mod.quantity;
            }
            else if (modOperation == SELL && m_sellsMap.count(orderID) > 0){
                Order& orderRef = m_sellsMap[orderID];
                orderRef.price = mod.price;
                orderRef.quantity = mod.quantity;
            }
            else if (modOperation == SELL && m_buysMap.count(orderID) > 0){
                // move from buy to sell with new data
                Order order = m_buysMap[orderID];
                order.price = mod.price;
                order.quantity = mod.quantity;
                m_buysMap.erase(orderID);
                m_sellsMap[orderID] = order;
            }
            else if (modOperation == BUY && m_sellsMap.count(orderID) > 0){
                // move from sell to buy with new data
                Order order = m_sellsMap[orderID];
                order.price = mod.price;
                order.quantity = mod.quantity;
                m_sellsMap.erase(orderID);
                m_buysMap[orderID] = order;
            }
        }
    }
    
    void print() const
    {
        // create ordered maps of <price, quantity>, where matching prices are summed
        // TODO: store these maps separately to avoid regenerating every time print is called
        const map<int, int> sortedSells = sortMapByPrice(m_sellsMap);
        const map<int, int> sortedBuys = sortMapByPrice(m_buysMap);

        cout << "SELL:" << endl;
        reversePrintMap(sortedSells);
        
        cout << "BUY:" << endl;
        reversePrintMap(sortedBuys);
        cout << endl;
    }
    
    private:
    // TODO: add timestamp to Order definition for cross-referencing with time-ordered map
    unordered_map<string, Order> m_buysMap;
    unordered_map<string, Order> m_sellsMap;

    // <price, {quantity, ID}>
    // NOTE: These need to be multimaps as we can have multiple entries with
    // the same price
    map<int, pair<int, string>> m_buysPriceOrderedMap;
    map<int, pair<int, string>> m_sellsPriceOrderedMap;

    // TODO: change this to use global order count instead of timestamp to ensure
    // unique
    map<time_t, PriceQuantityID> m_buysTimeOrderedMap;
    map<time_t, PriceQuantityID> m_sellsTimeOrderedMap;
    
    static map<int, int> sortMapByPrice(
        const unordered_map<string, Order>& inMap)
    {
        map<int, int> orderedMap;
        for (auto [key, val] : inMap) {
            const int newKey = val.price;
            if (orderedMap.count(newKey) == 0) {
                orderedMap[newKey] = val.quantity;
            }
            else {
                // sum entries with the same price
                orderedMap[newKey] += val.quantity;
            }
        }
        return orderedMap;
    }

    static void printTrade(const PriceQuantityID& a, const PriceQuantityID& b)
    {
        cout << "TRADE " << a.orderID << " " << a.price << " " << a.quantity <<
            " " << b.orderID << " " << b.price << " " << b.quantity << endl;
    }

    int checkPriceCrossOnBuy(const PriceQuantityID& buyInfo)
    {
        int remainingBuyQty = buyInfo.quantity;

        // check for price cross to skip unnecessary looping over maps
        const int minSellsPrice = m_sellsPriceOrderedMap.begin()->first;
        if (buyInfo.price >= minSellsPrice) {

            // iterate over sells time map, checking for prices <= buy price
            // this ensures we choose the earliest order first
            for (auto [time, sellInfo] : m_sellsTimeOrderedMap) {
                if (sellInfo.price < buyInfo.price) {
                    const int tradeQty = min(remainingBuyQty, sellInfo.quantity);
                    remainingBuyQty -= tradeQty;
                    // print trade
                    printTrade({ sellInfo.price, tradeQty, sellInfo.orderID },
                        { buyInfo.price, tradeQty, buyInfo.orderID });
                    // cancel or subtract traded sells
                    if (tradeQty == sellInfo.quantity) {
                        cancel(sellInfo.orderID);
                    }
                    else {
                        Modification mod(SELL, sellInfo.price, sellInfo.quantity - tradeQty);
                        modify(sellInfo.orderID, mod);
                    }
                }
                if (remainingBuyQty <= 0) {
                    break;
                }
            }
        }
            // if found:
            // - append orders involved in trade
            // - update all maps to subtract sells involved
            // - if sell.quantity < buy.quantity, continue to next order
            // break once exhausted
        return remainingBuyQty;
    }

    int checkPriceCrossOnSell(const PriceQuantityID& sellInfo)
    {
        int remainingSellQty = sellInfo.quantity;

        // check for price cross to skip unnecessary looping over maps
        const int maxBuysPrice = m_buysPriceOrderedMap.rbegin()->first;
        if (sellInfo.price <= maxBuysPrice) {

            // iterate over buys time map, checking for prices >= sell price
            // this ensures we choose the earliest order first
            for (auto [time, buyInfo] : m_buysTimeOrderedMap) {
                if (sellInfo.price < buyInfo.price) {
                    const int tradeQty = min(remainingSellQty, buyInfo.quantity);
                    remainingSellQty -= tradeQty;
                    // print trade
                    printTrade({ buyInfo.price, tradeQty, buyInfo.orderID },
                        { sellInfo.price, tradeQty, sellInfo.orderID });
                    // cancel or subtract traded buys
                    if (tradeQty == buyInfo.quantity) {
                        cancel(buyInfo.orderID);
                    }
                    else {
                        Modification mod(BUY, buyInfo.price, buyInfo.quantity - tradeQty);
                        modify(buyInfo.orderID, mod);
                    }
                }
                if (remainingSellQty <= 0) {
                    break;
                }
            }
        }
            // if found:
            // - append orders involved in trade
            // - update all maps to subtract sells involved
            // - if sell.quantity < buy.quantity, continue to next order
            // break once exhausted
        return remainingSellQty;
    }

    static void reversePrintMap(const map<int, int>& inMap)
    {
        for (auto it = inMap.rbegin(); it != inMap.rend(); ++it) {
            cout << it->first << " " << it->second << endl;
        }
    }

    bool shouldRecord(const string& orderID, const Order& order)
    {
        return isValid(orderID, order) && order.type != IOC;
    }

    bool isValid(const string& orderID, const Order& order)
    {
        return !orderExists(orderID) && order.price > 0 && order.quantity > 0;
    }
    
    bool orderExists(const string& orderID)
    {
        return m_buysMap.count(orderID) > 0 || m_sellsMap.count(orderID) > 0;
    }
    
    bool canModify(const string& orderID, const Modification& mod)
    {
        return ((m_buysMap.count(orderID) > 0 && m_buysMap[orderID].type != IOC)
            || (m_sellsMap.count(orderID) > 0 && m_sellsMap[orderID].type != IOC))
            && (mod.operation == BUY || mod.operation == SELL);
    }
};

/**
* @brief This class is responsible for handling inputs and sanitizing,
* filtering incorrectly formatted inputs.
*/
class InputHandler
{
    public:
    InputHandler() : m_orderMgr()
    {
        // do nothing
    }
    
    /**
    * @brief This processes a new line to execute the appropriate action.
    *
    * @param line The command line to process.
    */
    void process(const string& line)
    {
        istringstream lineStream(line);
        vector<string> commands;
        string chunk;
        // TODO: trim off trailing white space
        while (lineStream.good()) {
            getline(lineStream, chunk, DELIM);
            commands.emplace_back(chunk);
        }
        
        // TODO: move the below code into the OrderManager, as this class
        // shouldn't be responsible or aware of these details...
        // if empty, nothing to do and silently exit
        if (!commands.empty()) {
        
            // determine type of operation based on first entry
            // convert to upper-case to avoid case-sensitivity
            
            const operation_type opType = assignOperationType(commands[0]);
            if (opType == BUY) {
                if (commands.size() != 5) {
                    // exit silently with invalid inputs
                    return;
                }
                const auto [id, order] = assignOrderFrom(commands);
                m_orderMgr.buy(id, order);
            }
            else if (opType == SELL) {
                if (commands.size() != 5) {
                    // exit silently with invalid inputs
                    return;
                }
                const auto [id, order] = assignOrderFrom(commands);
                m_orderMgr.sell(id, order);
            }
            else if (opType == CANCEL) {
                if (commands.size() != 2) {
                    // exit silently with invalid inputs
                    return;
                }
                const string id = commands[1];
                m_orderMgr.cancel(id);
            }
            else if (opType == MODIFY) {
                if (commands.size() != 5) {
                    // exit silently with invalid inputs
                    return;
                }
                const auto [id, modification] = assignModificationFrom(commands);
                m_orderMgr.modify(id, modification);
            }
            else if (opType == PRINT) {
                if (commands.size() != 1) {
                    // exit silently with invalid inputs
                    return;
                }
                m_orderMgr.print();
            }
            // silently do nothing if not one of the above
        }
        // check for invalid inputs and ignore
    }
    
    private:
    OrderManager m_orderMgr;
    
    string toUpper(string str)
    {
        transform(str.begin(), str.end(), str.begin(), ::toupper); 
        return str;
    }
    
    pair<string, Order> assignOrderFrom(const vector<string>& commands)
    {
        const string orderTypeStr = toUpper(commands[1]);
        
        // TODO: add error handling if neither?
        order_type type = (orderTypeStr == IOC_STR) ? IOC : GFD;
        const int price = static_cast<int>(stoi(commands[2]));
        const int quantity = static_cast<int>(stoi(commands[3]));
        const string id = commands[4];
        const time_t timeStamp = time(0);
        return make_pair(id, Order(type, price, quantity));
    }
    
    pair<string, Modification> assignModificationFrom(const vector<string>& commands)
    {
        const string id = commands[1];
        const operation_type opType = assignOperationType(commands[2]);
        const int price = static_cast<int>(stoi(commands[3]));
        const int quantity = static_cast<int>(stoi(commands[4]));
        return make_pair(id, Modification(opType, price, quantity));
    }
    
    operation_type assignOperationType(string opString)
    {
        opString = toUpper(opString);
        operation_type opType = UNSET;
        if (opString == BUY_STR) {
            opType = BUY;
        }
        else if (opString == SELL_STR) {
            opType = SELL;
        }
        else if (opString == CANCEL_STR) {
            opType = CANCEL;
        }
        else if (opString == MODIFY_STR) {
            opType = MODIFY;
        }
        else if (opString == PRINT_STR) {
            opType = PRINT;
        }
        return opType;
    }
};

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    // continuously monitor stdin
    InputHandler iHandler;
    for (string line; getline(cin, line);) {
        // pass line to the input handler
        iHandler.process(line);
    }
    return 0;
}