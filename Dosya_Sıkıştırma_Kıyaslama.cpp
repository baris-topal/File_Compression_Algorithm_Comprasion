#include <iostream>
#include <string>
#include <fstream>
#include<map>
#include<queue>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <iterator>

using namespace std;

map<char, string> codes;
map<char, int> freq;

struct HNode
{
	char c;
	int freq;
	HNode* left, * right;
	HNode(char data, int freq)
	{
		left = right = NULL;
		this->c = data;
		this->freq = freq;
	}
};
struct compare
{
	bool operator()(HNode* l, HNode* r)
	{
		return (l->freq > r->freq);
	}
};

void printCodes(struct HNode* root, string str)
{
	if (!root)
		return;
	if (root->c != '$')
		cout << root->c << ": " << str << "\n";
	printCodes(root->left, str + "0");
	printCodes(root->right, str + "1");
}

void storeCodes(struct HNode* root, string str)
{
	if (root == NULL)
		return;
	if (root->c != '$')
		codes[root->c] = str;
	storeCodes(root->left, str + "0");
	storeCodes(root->right, str + "1");
}

priority_queue<HNode*, vector<HNode*>, compare> prique;

void HuffmanCodes(int size)
{
	struct HNode* left, * right, * top;
	for (map<char, int>::iterator v = freq.begin(); v != freq.end(); v++)
		prique.push(new HNode(v->first, v->second));
	while (prique.size() != 1)
	{
		left = prique.top();
		prique.pop();
		right = prique.top();
		prique.pop();
		top = new HNode('$', left->freq + right->freq);
		top->left = left;
		top->right = right;
		prique.push(top);
	}
	storeCodes(prique.top(), "");
}

void calcFreq(string str, int n)
{
	for (int i = 0; i < str.size(); i++)
		freq[str[i]]++;
}

string decode_file(struct HNode* root, string s)
{
	string ans = "";
	struct HNode* curr = root;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == '1')
			curr = curr->left;
		else
			curr = curr->right;

		if (curr->left == NULL and curr->right == NULL)
		{
			ans += curr->c;
			curr = root;
		}
	}
	return ans + '\0';
}

struct LNode
{
	int index;
	string data;
	LNode* next;
};


void st_Node(LNode* head, int index, string data)
{
	head->index = index;
	head->data = data;
	head->next = NULL;
}

void insert_Node(LNode* head, int index, string data)
{
	LNode* new_Node = new LNode;
	new_Node->index = index;
	new_Node->data = data;
	new_Node->next = NULL;

	LNode* curr = head;
	while (curr != NULL)
	{
		if (curr->next == NULL)
		{
			curr->next = new_Node;
			return;
		}
		curr = curr->next;
	}
}

LNode* search_Node(LNode* head, string data)
{
	LNode* curr = head;
	while (curr != NULL)
	{
		if (data.compare(curr->data) == 0)
			return curr;
		else
			curr = curr->next;
	}
	return NULL;
}

vector <string> split(string str, char delimiter)
{
	vector<string> internal;
	stringstream ss(str);
	string tok;

	while (getline(ss, tok, delimiter))
		internal.push_back(tok);
	return internal;
}

string LZ78(string input)
{
	LNode* dictionary = new LNode;
	string word, result;
	int length, last_seen, index = 1;

	length = (int)input.length();
	word = input[0];
	st_Node(dictionary, 1, word);
	//result += "0" + word;
	for (int i = 1; i < length; i++)
	{
		string data;
		data = input[i];

	re_check:
		LNode* search = search_Node(dictionary, data);

		if (search)
		{
			i++;
			data += input[i];
			last_seen = search->index;
			goto re_check;
		}
		else
		{
			char zero;
			if (input[i] == ' ')
				zero = '0';
			else
				zero = input[i];

			if ((int)data.length() < 2)
				if (i == 1)
					result += to_string(0) + ":" + zero;
				else
					result += " " + to_string(0) + ":" + zero;

			else
				result += " " + to_string(last_seen - 1) + ":" + zero;

			index++;
			if (i != length)
				insert_Node(dictionary, index, data);
		}
	}
	return result;
}

int huffman_hesapla(string str)
{
	int size;
	size = str.size();
	return size;
}


void tokenize(const string& s, const char delim, vector<string>& out)
{
	string::size_type beg = 0;
	for (auto end = 0; (end = s.find(delim, end)) != string::npos; ++end)
	{
		out.push_back(s.substr(beg, end - beg));
		beg = end + 1;
	}
	out.push_back(s.substr(beg));
}

string binary_converter(int num)
{
	if (num == 0) {
		return "0";
	}
	else
	{
		vector<int>v;
		string bin = "";
		int n = 0;
		while (num != 0)
		{
			v.push_back(num % 2);
			num = num / 2;
			n++;
		}
		reverse(v.begin(), v.end());
		for (int i = 0; i < n; i++)
		{
			bin += to_string(v[i]);
		}
		return bin;
	}
}

int lz_hesapla(string str)
{
	int size = 0;
	istringstream buf(str);
	istream_iterator<string> beg(buf), end;
	vector<string> tokens(beg, end);
	for (auto& s : tokens)
	{

		vector<string> out;
		tokenize(s, ':', out);

		size += binary_converter(atoi(out[0].c_str())).size();
		size += 8;

	}
	return size;
}

int main() {
	string temp, decoded, hufman_encoded, lz_encoded;
	ifstream out("metin.txt", ios::binary);
	if (!out.is_open())
	{
		exit(0);
	}
	else
	{
		while (!out.eof())
		{
			getline(out, temp);
			decoded += temp;
		}
		out.close();
	}

	calcFreq(decoded, decoded.length());
	HuffmanCodes(decoded.length());
	for (auto i : decoded)
		hufman_encoded += codes[i];
	ofstream in("huffman_text.txt", ios::binary);
	if (in.is_open())
	{
		in << hufman_encoded;
		in.close();
	}
	else
	{
		exit(0);
	}

	lz_encoded = LZ78(decoded);
	in.open("LZ78_text.txt", ios::binary);
	if (in.is_open())
	{
		in << lz_encoded;
		in.close();
	}
	else
	{
		exit(0);
	}
	int orjinal = decoded.size() * 8, huffman = huffman_hesapla(hufman_encoded), lz_78 = lz_hesapla(lz_encoded);
	cout << "Boyutlar" << endl;
	cout << "--------" << endl;
	cout << "Metin dosyasi orjinal boyut " << orjinal << " bittir." << endl;
	cout << "Huffman sonucu boyut " << huffman << " bittir." << endl;
	cout << "LZ78 sonucu boyut " << lz_78 << " bittir." << endl << endl;;

	cout << "Verimler" << endl;
	cout << "--------" << endl;
	cout << "Hauffman Verim: %" << 100 - ((huffman * 100) / orjinal) << endl;
	cout << "LZ78 Verim: %" << 100 - ((lz_78 * 100) / orjinal) << endl;

	system("pause>null");
	return 0;
}
