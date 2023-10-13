#include<bits/stdc++.h>
using namespace std;

class Process // one process
{
private:
    int index; // name?
    int arrival;
    int burst;
    int priority;
public:
    // constructor
    Process()
    {
        this->index = -1;
        this->arrival = -1;
        this->burst = -1;
        this->priority = -1;
    }
    Process(int x, int y, int z, int t)
    {
        this->index   = x;
        this->arrival = y;
        this->burst   = z;
        this->priority= t;
    }
    int _index()
    {
        return this->index;
    }
    int _arrival()
    {
        return this->arrival;
    }
    int _burst()
    {
        return this->burst;
    }
    int _priority()
    {
        return this->priority;
    }
    friend ostream& operator<<(ostream& os, Process& P)
    {
        os << "P[" << P.index << "]\t:\t|" << P.arrival << "\t|" << P.burst << "\t|" << P.priority;
        return os;
    }
};

class Solution
{
    // can gi bo vao day
public:
    bool isNum(char c)
    {
        for(char x='0' ; x<='9' ; x++)
        {
            if(c==x)
                return true;
        }
        return false;
    }
    string numToString(int x) // x>=0
    {
        string answer = "";
        if(x == 0)
            return "0";
        else
        {
            while(x > 0)
            {
                answer = char(x%10 + 48) + answer;
                x /= 10;
            }
            return answer;
        }
    }
    int stringToNum(string s)
    {
        if(s == "0")
            return 0;
        int ans = 0;
        for(auto i : s)
        {
            ans = ans*10 + int(i)-48;
        }
        return ans;
    }
    vector<string> convertGantt(string &s)
    {
        stack<string> box;
        vector<string> newBox;
        for(int i=0 ; i<s.size() ; i++)
        {
            if(isNum(s[i]))
            {
                string tmp = "";
                tmp += s[i++];
                while(isNum(s[i]))
                    tmp += s[i++];
                box.push(tmp);
            }
            if(s[i] == '_')
                continue;
            if(s[i] == 'P')
            {
                string tmp = "";
                tmp += s[i++];
                while(s[i] != '_')
                    tmp += s[i++];
                box.push(tmp);
            }
        }
        string lastP = "";
        while(!box.empty())
        {
            if(box.top()[0] == 'P')
            {
                if(box.top() == lastP)
                {
                    box.pop();
                    newBox.erase(newBox.begin());
                }
                else
                {
                    newBox.insert(newBox.begin()+0, box.top());
                    box.pop();
                    lastP = newBox[0];
                }
            }
            else
            {
                newBox.insert(newBox.begin() + 0, box.top());
                box.pop();
            }
        }
        return newBox;
    }
    void ___printVec(vector<string> box) // tmp
    {
        for(auto i : box)
            cout << i << "\n";
    }
    void ___printMap(map<pair<int, int>, int> box)
    {
        for(auto i : box)
        {
            //cout << i.first << " : [" << i.second.first << "," << i.second.second << "]\n";
            cout << "[" << i.first.first << "," << i.first.second << "] : " << i.second << "\n";
        }
    }
    //float completionTime()
    vector<float> calculateTime(map<pair<int,int>, int> &gantt_map, vector<Process> &p)
    {
        // completion time
        map<int, float> completionTime;
        for(auto i : gantt_map)
        {
            completionTime[i.second] = i.first.second;
        }
        float totalCompletionTime = 0;
        for(auto i : completionTime)
        {
            totalCompletionTime += i.second;
        }
        float aveCompletionTime = totalCompletionTime / completionTime.size();
        // turn around time
        map<int, float> turnAroundTime;
        float totalTurnAroundTime = 0;
        for(auto i: completionTime)
        {
            turnAroundTime[i.first] = i.second - p[i.first-1]._arrival();
            totalTurnAroundTime += turnAroundTime[i.first];
        }
        float aveTurnAroundTime = totalTurnAroundTime / turnAroundTime.size();
        // waiting time
        map<int, float> waitingTime;
        float totalWaitingTime = 0;
        for(auto i : turnAroundTime)
        {
            waitingTime[i.first] = i.second - p[i.first-1]._burst();
            totalWaitingTime += waitingTime[i.first];
        }
        float aveWaitingTime = totalWaitingTime / waitingTime.size();

        return {aveCompletionTime, aveTurnAroundTime, aveWaitingTime};
    }
};

class shortestJobFirst
{
private:
    float aveWaitingTime;    // thoi gian doi trung binh
    float aveCompletionTime; // thoi gian hoan thanh trung binh
    float aveTurnAroundTime; // thoi gian dap ung trung binh
    string gantt_str;        // giản đồ gantt, dạng chuỗi
    vector<string> gantt_vec;// giản đồ gantt, dạng vector
    map<pair<int,int>, int> gantt_map; // {{start, finish}, index}
    // giản đồ gantt, dạng map (recomended)
public:
    // constructor
    shortestJobFirst() {}
    shortestJobFirst(vector<Process> &p)
    {
        string s = "";
        map<int, bool> check;
        vector<pair<int, int>> box; //{ index, P[index]'s burst time}
        int time = p[0]._arrival();
        s += Solution().numToString(p[0]._arrival());
        box.push_back({p[0]._index(), p[0]._burst()});
        check[p[0]._index()] = true;
        while(!box.empty())
        {
            time++;
            box[0].second--;
            s += "_P[" + Solution().numToString(box[0].first) + "]_" + Solution().numToString(time);
            if(box[0].second <=0)
            {
                box.erase(box.begin() + 0);
            }
            for(int i=0 ; i<p.size() ; i++)
            {
                if(check[p[i]._index()] == true)
                    continue;
                else if(p[i]._arrival() == time)
                {
                    box.push_back({p[i]._index(), p[i]._burst()});
                    check[p[i]._index()] = true;
                    // sort the box by burst time
                    for(int j=0 ; j<box.size()-1 ; j++)
                        for(int k=j+1 ; k<box.size() ; k++)
                            if(box[j].second > box[k].second)
                                swap(box[j], box[k]);
                }
            }
        }
        //
        this->gantt_vec = Solution().convertGantt(s);
        for(int i=0 ; i<this->gantt_vec.size() ; i++)
        {
            this->gantt_str += this->gantt_vec[i];
            if(i < this->gantt_vec.size()-1)
                this->gantt_str += "_";
        }
        // gantt_map
        for(int i=1 ; i<this->gantt_vec.size()-1 ; i+=2)
        {
            // map<pair<int, int>, int>
            //pair<int, pair<int, int>> tmp;
            pair<int, int> tmp;
            tmp = {Solution().stringToNum(this->gantt_vec[i-1]), Solution().stringToNum(this->gantt_vec[i+1])};
            // get the number part in "P[123...]"
            //string tmp_str = "";
            int tmp_index =  0;
            //tmp.first = 0;
            int j = 2;
            while(Solution().isNum(this->gantt_vec[i][j]) == true)
            {
                tmp_index = tmp_index*10 + int(this->gantt_vec[i][j]) -48;
                j++;
            }
            //this->gantt_map.push_back(tmp);
            gantt_map[tmp] = tmp_index;
        }

        vector<float> averageTime = Solution().calculateTime(this->gantt_map, p);
        this->aveCompletionTime = averageTime[0];
        this->aveTurnAroundTime = averageTime[1];
        this->aveWaitingTime    = averageTime[2];
    }

    //constructor
    vector<string> _gantt_vec() // lấy giá trị của gantt_vec
    {
        return this->gantt_vec;
    }
    string _gantt_str()
    {
        return this->gantt_str;
    }
    map<pair<int, int>, int> _gantt_map()
    {
        return this->gantt_map;
    }
    float _aveCompletionTime()
    {
        return this->aveCompletionTime;
    }
    float _aveTurnAroundTime()
    {
        return this->aveTurnAroundTime;
    }
    float _aveWaitingTime()
    {
        return this->aveWaitingTime;
    }
    //
};

class roundRobin
{
private:
    float aveWaitingTime;    // thoi gian doi trung binh
    float aveCompletionTime; // thoi gian hoan thanh trung binh
    float aveTurnAroundTime; // thoi gian dap ung trung binh
    string gantt_str;        // giản đồ gantt, dạng chuỗi
    vector<string> gantt_vec;// giản đồ gantt, dạng vector
    map<pair<int,int>, int> gantt_map; // {{start, finish}, index}
    // giản đồ gantt, dạng map (recomended)
public:
    // constructor
    roundRobin() {};
    roundRobin(vector<Process> &p, int quantumTime)
    {
        // quantum time = 4?
        string s = ""; // first gantt chart as string
        map<int, bool> check;
        queue<pair<int, int>> box; // <index, burst time>
        int time = p[0]._arrival(); // first process
        box.push({p[0]._index(), p[0]._burst()}); // push_back
        // pop_front
        s += Solution().numToString(p[0]._arrival());
        int turning = 0;
        while(box.empty() == false)
        {
            time++;
            turning++;
            box.front().second--;
            s += "_P[" + Solution().numToString(box.front().first) + "]_" + Solution().numToString(time);

            for(int i=0 ; i<p.size() ; i++)
            {
                if(check[p[i]._index()] == true)
                    continue;
                else if(p[i]._arrival() == time)
                {
                    box.push({p[i]._index(), p[i]._burst()});
                    check[p[i]._index()] = true;
                    // no need to sort
                }
            }
            if(box.front().second <= 0)
            {
                box.pop(); // pop_front
                turning = 0;
                // https://www.quora.com/If-the-process-finished-before-a-time-quantum-in-round-robin-does-the-process-wait-for-the-given-time-quantum-or-does-it-get-executed-without-waiting
            }
            if(turning == quantumTime)
            {
                box.push(box.front());
                box.pop();
                turning = 0;
            }

        }
        this->gantt_vec = Solution().convertGantt(s);
        for(int i=0 ; i<this->gantt_vec.size() ; i++)
        {
            this->gantt_str += this->gantt_vec[i];
            if(i < this->gantt_vec.size()-1)
                this->gantt_str += "_";
        }
        // gantt map
        for(int i=1 ; i<this->gantt_vec.size()-1 ; i+=2)
        {
            pair<int, int> tmp_pair;
            tmp_pair = {Solution().stringToNum(this->gantt_vec[i-1]), Solution().stringToNum(this->gantt_vec[i+1])};
            int tmp_index = 0;
            int j = 2;
            while(Solution().isNum(this->gantt_vec[i][j]))
            {
                tmp_index = tmp_index*10 + int(this->gantt_vec[i][j])-48;
                j++;
            }
            gantt_map[tmp_pair] = tmp_index;
        }
        //
        //map<int, float> completionTime;
        vector<float> averageTime = Solution().calculateTime(this->gantt_map, p);
        this->aveCompletionTime = averageTime[0];
        this->aveTurnAroundTime = averageTime[1];
        this->aveWaitingTime    = averageTime[2];
    }
    vector<string> _gantt_vec()
    {
        return this->gantt_vec;
    }
    string _gantt_str()
    {
        return this->gantt_str;
    }
    map<pair<int,int>, int> _gantt_map()
    {
        return this->gantt_map;
    }
    float _aveCompletionTime()
    {
        return this->aveCompletionTime;
    }
    float _aveTurnAroundTime()
    {
        return this->aveTurnAroundTime;
    }
    float _aveWaitingTime()
    {
        return this->aveWaitingTime;
    }
};

class prioritySchedule
{
private:
    float aveWaitingTime;    // thoi gian doi trung binh
    float aveCompletionTime; // thoi gian hoan thanh trung binh
    float aveTurnAroundTime; // thoi gian dap ung trung binh
    string gantt_str;        // giản đồ gantt, dạng chuỗi
    vector<string> gantt_vec;// giản đồ gantt, dạng vector
    map<pair<int,int>, int> gantt_map; // {{start, finish}, index}
    // giản đồ gantt, dạng map (recomended)
public:
    prioritySchedule() {}
    prioritySchedule(vector<Process> &p)
    {
        string s = "";
        map<int, bool> check;
        vector<vector<int>> box; // {index, arrival, burst, priority}
        box.push_back({p[0]._index(), p[0]._arrival(), p[0]._burst(), p[0]._priority()});
        int time = p[0]._arrival();
        s += Solution().numToString(p[0]._arrival());
        check[p[0]._index()] = true;
        while(!box.empty())
        {
            time++;
            //box[0]._burst() = box[0]._burst() - 1;
            //int tmp = box[0]._burst() - 1;
            //box[0]._burst() = tmp;
            // https://stackoverflow.com/questions/33375136/lvalue-required-as-left-operand-of-assignment-error-when-using-c
            box[0][2]--; // burst--
            s += "_P[" + Solution().numToString(box[0][1]) + "]_" + Solution().numToString(time);
            if(box[0][2] <= 0)
                box.erase(box.begin() + 0);
            for(int i=0 ; i<p.size() ; i++)
            {
                if(check[p[i]._index()] == true)
                    continue;
                else if(p[i]._arrival() == time)
                {
                    box.push_back({p[i]._index(), p[i]._arrival(), p[i]._burst(), p[i]._priority()});
                    check[p[i]._index()] = true;
                    // sort the box by priorities
                    for(int j=0 ; j<box.size()-1 ; j++)
                        for(int k=j+1 ; k<box.size() ; k++)
                            if(box[j][3] > box[k][3]) // priority
                                swap(box[k], box[j]);
                }
            }
            // gantt
            this->gantt_vec = Solution().convertGantt(s);
            for(int i=0 ; i<this->gantt_vec.size() ; i++)
            {
                this->gantt_str += this->gantt_vec[i];
                if(i < this->gantt_vec.size()-1)
                    this->gantt_str += "_";
            }
            for(int i=1 ; i<this->gantt_vec.size()-1 ; i+=2)
            {
                pair<int, int> tmp;
                tmp = {Solution().stringToNum(this->gantt_vec[i-1]), Solution().stringToNum(this->gantt_vec[i+1])};
                // get the number part in "P[123...]"
                //string tmp_str = "";
                int tmp_index =  0;
                //tmp.first = 0;
                int j = 2;
                while(Solution().isNum(this->gantt_vec[i][j]) == true)
                {
                    tmp_index = tmp_index*10 + int(this->gantt_vec[i][j]) -48;
                    j++;
                }
                //this->gantt_map.push_back(tmp);
                gantt_map[tmp] = tmp_index;
            }
            vector<float> averageTime = Solution().calculateTime(this->gantt_map, p);
            this->aveCompletionTime = averageTime[0];
            this->aveTurnAroundTime = averageTime[1];
            this->aveWaitingTime    = averageTime[2];
        }

    }
    // constructor
    vector<string> _gantt_vec()
    {
        return this->gantt_vec;
    }
    string _gantt_str()
    {
        return this->gantt_str;
    }
    map<pair<int,int>, int> _gantt_map()
    {
        return this->gantt_map;
    }
    float _aveCompletionTime()
    {
        return this->aveCompletionTime;
    }
    float _aveTurnAroundTime()
    {
        return this->aveTurnAroundTime;
    }
    float _aveWaitingTime()
    {
        return this->aveWaitingTime;
    }
};

void Action(vector<Process> &p)
{
    cout << "choose your arglorithm (0, 1 or 2)\n";
    cout << "0. Shortest job first (preemptive)\n1. Round robin\n2. Priority scheduling\n";
    int x;
    cin >> x;
    switch(x)
    {
    case 0:
        {
            cout << "Shortest job first (preemptive)\n";
            shortestJobFirst A(p);
            cout << "Gantt chart :\n";
            cout << A._gantt_str() << "\n";
            cout << "Average completion time : " << A._aveCompletionTime() << "\n";
            cout << "Average turn around time: " << A._aveTurnAroundTime() << "\n";
            cout << "Average waiting time    : " << A._aveWaitingTime()    << "\n";
            cout << "---\n";
            break;
        }
    case 1:
        {
            cout << "Round robin\n";
            cout << "quantum time : ";
            int quantumTime;
            cin >> quantumTime;
            roundRobin A(p, quantumTime);
            cout << "Gantt chart :\n";
            cout << A._gantt_str() << "\n";
            cout << "Average completion time : " << A._aveCompletionTime() << "\n";
            cout << "Average turn around time: " << A._aveTurnAroundTime() << "\n";
            cout << "Average waiting time    : " << A._aveWaitingTime()    << "\n";
            cout << "---\n";
            break;
        }
    case 2:
        {
            cout << "Priority cheduling\n";
            prioritySchedule A(p);
            cout << "Gantt chart :\n";
            cout << A._gantt_str() << "\n";
            cout << "Average completion time : " << A._aveCompletionTime() << "\n";
            cout << "Average turn around time: " << A._aveTurnAroundTime() << "\n";
            cout << "Average waiting time    : " << A._aveWaitingTime()    << "\n";
            cout << "---\n";
            break;
        }
    }
}

int main()
{
    cout << "number of process : ";
    int n; // number of processes
    cin >> n;
    system("cls");
    vector<Process> P(n);
    int arrival, burst, priority;
    for(int i=0 ; i<n ; i++)
    {
        cout << "arrival[" << i+1 << "] : ";
        cin >> arrival;
        cout << "burst[" << i+1 << "] : ";
        cin >> burst;
        cout << "priority[" << i+1 << "] : ";
        cin >> priority;
        //cin >> arrival >> burst >> priority;
        P[i] = Process(i+1, arrival, burst, priority);
        system("cls");
    }
    system("cls");
    cout << "processes\tarrival\tburst\tpriority\n";
    for(auto i : P)
        cout << i << "\n";
    cout << "----------\n";
    Action(P);

}

/* test case
5
0
10
3
1
3
2
2
2
1
3
1
2
4
5
4
*/
