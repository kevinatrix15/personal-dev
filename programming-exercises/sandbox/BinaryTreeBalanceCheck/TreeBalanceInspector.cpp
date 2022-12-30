#include <algorithm>
#include <iostream>

using namespace std;

struct TreeNode
{
public:
	int val;
	TreeNode* left;
	TreeNode* right;

	TreeNode(int val) : val(val), left(nullptr), right(nullptr)
	{
	}
};

class TreeBalanceInspector
{
public:
	bool isBalanced(TreeNode* root)
	{
		return checkBalance(root);
	}

private:
	int getDepth(TreeNode* node)
	{
		if (!node) {
			return 0;
		}

		const int leftDepth = getDepth(node->left);
		const int rightDepth = getDepth(node->right);
		return max(leftDepth, rightDepth) + 1;
	}

	bool checkBalance(TreeNode* node)
	{
		if (!node) {
			return true;
		}

		if (abs(getDepth(node->left) - getDepth(node->right)) <= 1) {
			const bool leftBal = checkBalance(node->left);
			const bool rightBal = checkBalance(node->right);
			return leftBal && rightBal;
		} else {
			return false;
		}
	}
};

int main()
{
	TreeNode* root = new TreeNode(7);
	root->left = new TreeNode(3);
	// root->left->left = new TreeNode(1);
	root->left->right = new TreeNode(4);
	root->left->right->right = new TreeNode(8);

	root->right = new TreeNode(13);
	//root->right->left = new TreeNode(11);
	//root->right->right = new TreeNode(20);
	TreeBalanceInspector tbi;
	cout << "balanced: " << tbi.isBalanced(root) << endl;
}
'


//Jeremy's includes
#include <stdio.h>
#include <stdint.h>
#include <string.h>

//Kevin's includes
#include <sstream>
#include <string>

using namespace std;

#if 0
string readFileIntoString(const string& path) {
    auto ss = ostringstream{};
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}
#endif

#if 0
readCsvData()
{
  // do stuff...
}
#endif

int main()
{
    // Read files
    FILE *fpos;

    fpos = fopen("/home/coderpad/data/position_xy.csv", "r");
    // error handling?
    // fpos will == NULL if the file doesn't exist or can't be opened

    // Loop through position file 
    char line[4096];
    fgets(line, 4095, fpos); // Very hacky skip line 1
    fgets(line, 4095, fpos); // Very hacky skip line 2

    vector<double> output;

    double pos_time = 0.0f;
    int num_positions = 0;
    double null_val = -9999.0;
    while(fgets(line, 4095, fpos) != NULL)
    {
        // terminate string in case the read overruns the size of line[4096]
        line[4095] = '\0';

        int ret;
        double pos_x, pos_y;

        ret = sscanf(line, "%lf,%lf", &pos_x, &pos_y);
        if(ret == 2) 
        {
            // Do something with pos_x and pos_y

            pos_time += 10.0f; // increase by 10msec
            num_positions++;
        }

        output.emplace_back(pos_time);
        output.emplace_back(pos_x);
        output.emplace_back(pos_y);
        output.emplace_back(null_val);

        // then use sscanf() to look for the X,Y positions
    }    
    fclose(fpos);

    vector<sensor_struct> sensor_data;
    // read sensor file
      // sensor_time = <read_time>
      // sensor_val = <read_val>
    
    // iterate over xy-position data, and find nearest sensor data to assign to output


    // output data structure:
    // {time, x, y, sensor}

    // Interpolate and plot data
    
    

    // Exit the program
    return 0;
