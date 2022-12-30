// Function to find majority element in the array
// a: input array
// size: size of input array
int majorityElement(int a[], int size)
{
    // your code here
    std::unordered_map<int, int> vals;
    for (size_t i = 0; i < size; ++i) {
        const int val = a[i];
        std::unordered_map<int, int>::iterator it = vals.find(val);
        if (it == vals.end()) {
            vals.emplace(std::make_pair(val, 1));
        } else {
            it->second++;
        }
    }
}
