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

    Order() : type(GFD), price(0), quantity(0)
    {
        // do nothing
    }

    Order(const order_type& type, const int price, const int quantity) :
        type(type), price(price), quantity(quantity)
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

struct PriceID
{
    int price;
    string orderID;

    PriceID() : price(0), orderID("")
    {
        // do nothing
    }

    PriceID(const int price, const string orderID) :
        price(price), orderID(orderID)
    {
        // do nothing
    }

    bool operator==(const PriceID& other) {
        return price == other.price && orderID == other.orderID;
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

class OrderManager
{
    public:
    OrderManager() : m_buysMap(), m_sellsMap(), m_buysPriceQuantityMap(), m_sellsPriceQuantityMap(), m_buysTimePriorityMap(), m_sellsTimePriorityMap()
    {
        // do nothing
    }
    
    void buy(const string& orderID, const Order& order)
    {
        const int updatedQty = checkPriceCrossOnBuy({order.price, order.quantity, orderID});
        const Order updatedOrder(order.type, order.price, updatedQty);

        // if order.type == IOC, only process if can be traded immediately. If only partially traded,
        // remainder should be canceled
        // ignore non-unique order IDs
        if (shouldRecord(orderID, updatedOrder)) {
            m_buysMap[orderID] = updatedOrder;       
            m_buysTimePriorityMap.emplace_back(PriceID{ order.price, orderID });

            if (m_buysPriceQuantityMap.count(updatedOrder.price) > 0) {
                m_buysPriceQuantityMap[updatedOrder.price] += updatedOrder.quantity;
            }
            else {
                m_buysPriceQuantityMap[updatedOrder.price] = updatedOrder.quantity;
            }
        }
    }
    
    void sell(const string& orderID, const Order& order)
    {
        const int updatedQty = checkPriceCrossOnSell({order.price, order.quantity, orderID});
        const Order updatedOrder(order.type, order.price, updatedQty);

        // if order.type == IOC, only process if can be traded immediately. If only partially traded,
        // remainder should be canceled
        // ignore non-unique order IDs
        if (shouldRecord(orderID, updatedOrder)) {
            m_sellsMap[orderID] = updatedOrder;       
            m_sellsTimePriorityMap.emplace_back(PriceID{ updatedOrder.price, orderID });

            if (m_sellsPriceQuantityMap.count(updatedOrder.price) > 0) {
                m_sellsPriceQuantityMap[updatedOrder.price] += updatedOrder.quantity;
            }
            else {
                m_sellsPriceQuantityMap[updatedOrder.price] = updatedOrder.quantity;
            }
        }
    }
    
    void cancel(const string& orderID)
    {
        if (m_buysMap.count(orderID) > 0) {
            const int price = m_buysMap[orderID].price;

            // clear price-quantity map
            const Modification cancelMod(BUY, 0, 0);
            modifyPriceQuantityMap(m_buysMap[orderID],
                cancelMod,
                m_buysPriceQuantityMap,
                m_buysPriceQuantityMap);

            // clear time-priority map
            const PriceID priceID(price, orderID);
            modifyTimePriorityMap(priceID, cancelMod, m_buysTimePriorityMap, m_buysTimePriorityMap);

            // remove from buys map
            m_buysMap.erase(orderID);
        }
        else if (m_sellsMap.count(orderID) > 0) {
            const int price = m_sellsMap[orderID].price;

            // clear price-quantity map
            const Modification cancelMod(SELL, 0, 0);
            modifyPriceQuantityMap(m_sellsMap[orderID],
                cancelMod,
                m_sellsPriceQuantityMap,
                m_sellsPriceQuantityMap);

            // clear time-priority map
            const PriceID priceID(price, orderID);
            modifyTimePriorityMap(priceID, cancelMod, m_sellsTimePriorityMap, m_sellsTimePriorityMap);

            // remove from sells map
            m_sellsMap.erase(orderID);
        }
    }
    
    // * Modify- Check order exists. Update operation, price, quantity. NOTE: can't modify IOC type! 
    void modify(const string& orderID, const Modification& mod)
    {
        if (canModify(orderID, mod)) {
            operation_type modOperation = mod.operation;
            // update all data structures / maps here
            if (modOperation == BUY && m_buysMap.count(orderID) > 0) {
                Order& orderRef = m_buysMap[orderID];

                modifyPriceQuantityMap(orderRef, mod, m_buysPriceQuantityMap, m_buysPriceQuantityMap);
                modifyTimePriorityMap(PriceID{ orderRef.price, orderID }, mod, m_buysTimePriorityMap, m_buysTimePriorityMap);
                modifyOrderMap(mod, orderRef);
            }
            else if (modOperation == SELL && m_sellsMap.count(orderID) > 0){
                Order& orderRef = m_sellsMap[orderID];

                modifyPriceQuantityMap(orderRef, mod, m_sellsPriceQuantityMap, m_sellsPriceQuantityMap);
                modifyTimePriorityMap(PriceID{ orderRef.price, orderID }, mod, m_sellsTimePriorityMap, m_sellsTimePriorityMap);
                modifyOrderMap(mod, orderRef);
            }
            else if (modOperation == SELL && m_buysMap.count(orderID) > 0){
                // move from buy to sell with new data
                const Order order = m_buysMap[orderID];

                modifyPriceQuantityMap(order, mod, m_buysPriceQuantityMap, m_sellsPriceQuantityMap);
                modifyTimePriorityMap(PriceID{ order.price, orderID }, mod, m_buysTimePriorityMap, m_sellsTimePriorityMap);
                modifyOrderMap(mod, orderID, m_buysMap, m_sellsMap);
            }
            else if (modOperation == BUY && m_sellsMap.count(orderID) > 0){
                // move from sell to buy with new data
                const Order order = m_sellsMap[orderID];

                modifyPriceQuantityMap(order, mod, m_sellsPriceQuantityMap, m_buysPriceQuantityMap);
                modifyTimePriorityMap(PriceID{ order.price, orderID }, mod, m_sellsTimePriorityMap, m_buysTimePriorityMap);
                modifyOrderMap(mod, orderID, m_buysMap, m_sellsMap);
            }
        }
    }
    
    void print() const
    {
        cout << "SELL:" << endl;
        reversePrintMap(m_sellsPriceQuantityMap);
        
        cout << "BUY:" << endl;
        reversePrintMap(m_buysPriceQuantityMap);
        cout << endl;
    }
    
    private:
    // <orderID, Order>
    unordered_map<string, Order> m_buysMap;
    unordered_map<string, Order> m_sellsMap;

    // <price, quantity>
    map<int, int> m_buysPriceQuantityMap;
    map<int, int> m_sellsPriceQuantityMap;

    vector<PriceID> m_buysTimePriorityMap;
    vector<PriceID> m_sellsTimePriorityMap;
    
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
        cout << endl;
    }

    // TODO: generalize this for use with both buys and sells
    int checkPriceCrossOnBuy(const PriceQuantityID& buyInfo)
    {
        if (m_sellsPriceQuantityMap.empty()) {
            return buyInfo.quantity;
        }

        int remainingBuyQty = buyInfo.quantity;

        // check for price cross to skip unnecessary looping over maps
        const int minSellsPrice = m_sellsPriceQuantityMap.begin()->first;
        if (buyInfo.price >= minSellsPrice) {

            // iterate over sells time map, checking for prices <= buy price
            // this ensures we choose the earliest order first
            vector<string> toCancel;
            for (size_t i = 0; i < m_sellsTimePriorityMap.size(); ++i) {
                const PriceID sellInfo = m_sellsTimePriorityMap[i];
                if (sellInfo.price <= buyInfo.price) {
                    // O(1) access of unordered_map
                    const int sellQty = m_sellsMap[buyInfo.orderID].quantity;
                    const int tradeQty = min(remainingBuyQty, sellQty);
                    remainingBuyQty -= tradeQty;

                    // print trade
                    printTrade({ sellInfo.price, tradeQty, sellInfo.orderID },
                        { buyInfo.price, tradeQty, buyInfo.orderID });

                    // cancel or subtract traded sells
                    if (tradeQty == sellQty) {
                        cancel(sellInfo.orderID);
                        // TODO: consider replacing entry with an invalid value to avoid memory reallocation
                        // evaluate tradeoffs on continuously increasing container and
                        // iteration over...
                        m_sellsTimePriorityMap.erase(m_sellsTimePriorityMap.begin() + i);
                    }
                    else {
                        Modification mod(SELL, sellInfo.price, sellQty - tradeQty);
                        modify(sellInfo.orderID, mod);
                    }
                }
                if (remainingBuyQty == 0) {
                    break;
                }
            }

            // cancel all empty orders
            for (auto id : toCancel) {
                cancel(id);
            }
        }
        return remainingBuyQty;
    }

    // TODO: generalize this for use with both buys and sells
    int checkPriceCrossOnSell(const PriceQuantityID& sellInfo)
    {
        if (m_buysPriceQuantityMap.empty()) {
            return sellInfo.quantity;
        }
        int remainingSellQty = sellInfo.quantity;

        // check for price cross to skip unnecessary looping over maps
        const int maxBuysPrice = m_buysPriceQuantityMap.rbegin()->first;
        if (sellInfo.price <= maxBuysPrice) {

            // iterate over buys time map, checking for prices >= sell price
            // this ensures we choose the earliest order first
            vector<string> toCancel;
            for (size_t i = 0; i < m_buysTimePriorityMap.size(); ++i) {
                const PriceID buyInfo = m_buysTimePriorityMap[i];
                if (sellInfo.price <= buyInfo.price) {
                    // O(1) access of unordered_map
                    const int buyQty = m_buysMap[buyInfo.orderID].quantity;
                    const int tradeQty = min(remainingSellQty, buyQty);
                    remainingSellQty -= tradeQty;

                    // print trade
                    printTrade({ buyInfo.price, tradeQty, buyInfo.orderID },
                        { sellInfo.price, tradeQty, sellInfo.orderID });

                    // cancel or subtract traded buys
                    if (tradeQty == buyQty) {
                        toCancel.emplace_back(buyInfo.orderID);
                    }
                    else {
                        Modification mod(BUY, buyInfo.price, buyQty - tradeQty);
                        modify(buyInfo.orderID, mod);
                    }
                }
                if (remainingSellQty == 0) {
                    break;
                }
            }

            // cancel all empty orders
            for (auto id : toCancel) {
                cancel(id);
            }
        }
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

    static void modifyPriceQuantityMap(const Order& orig,
									   const Modification& mod,
									   map<int, int>& srcPriceQtyMapRef,
									   map<int, int>& dstPriceQtyMapRef)
    {
	    if (srcPriceQtyMapRef.count(orig.price) > 0) {
			// remove old data
			srcPriceQtyMapRef[orig.price] -= orig.quantity;
			if (srcPriceQtyMapRef[orig.price] <= 0) {
				srcPriceQtyMapRef.erase(orig.price);
			}

			// update with modified data
            if (mod.price > 0 && mod.quantity > 0) {
                if (dstPriceQtyMapRef.count(mod.price) > 0) {
                    dstPriceQtyMapRef[mod.price] += mod.quantity;
                }
                else {
                    dstPriceQtyMapRef[mod.price] = mod.quantity;
                }
            }
		}
    }

    void modifyTimePriorityMap(const PriceID& orig,
                                const Modification& mod,
								vector<PriceID>& srcTimePriorityMapRef,
								vector<PriceID>& dstTimePriorityMapRef)
    {
		const auto it = find(srcTimePriorityMapRef.begin(), srcTimePriorityMapRef.end(), orig);
		if (it != srcTimePriorityMapRef.end()) {
			// erase existing entry
			srcTimePriorityMapRef.erase(it);
			// add new entry at back
			if (mod.price > 0) {
				dstTimePriorityMapRef.emplace_back(PriceID{ mod.price, orig.orderID });
			}
		}
	}

    void modifyOrderMap(const Modification& mod, Order& origRef)
    {
        origRef.price = mod.price;
        origRef.quantity = mod.quantity;
    }

    void modifyOrderMap(const Modification& mod,
                        const string orderID,
                        unordered_map<string, Order>& srcMapRef,
                        unordered_map<string, Order>& dstMapRef)
    {
        Order order = srcMapRef[orderID];
		order.price = mod.price;
		order.quantity = mod.quantity;
		srcMapRef.erase(orderID);
		dstMapRef[orderID] = order;
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
            if (opType == BUY && validNumArgs(5, commands)) {
                const auto [id, order] = assignOrderFrom(commands);
                m_orderMgr.buy(id, order);
            }
            else if (opType == SELL && validNumArgs(5, commands)) {
                const auto [id, order] = assignOrderFrom(commands);
                m_orderMgr.sell(id, order);
            }
            else if (opType == CANCEL && validNumArgs(2, commands)) {
                const string id = commands[1];
                m_orderMgr.cancel(id);
            }
            else if (opType == MODIFY && validNumArgs(5, commands)) {
                const auto [id, modification] = assignModificationFrom(commands);
                m_orderMgr.modify(id, modification);
            }
            else if (opType == PRINT && validNumArgs(1, commands)) {
                m_orderMgr.print();
            }
            // silently do nothing if not one of the above
        }
    }
    
    private:
    OrderManager m_orderMgr;

    static bool validNumArgs(const size_t expected, const vector<string>& args)
    {
        return expected == args.size();
    }
    
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