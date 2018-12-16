
module StationDemo{
	
	sequence<string> myTopicList;
	struct Message{
	
	string tpName;
	string data;
	string time;
	int flag; 
	};
	interface StationMessage{
		void getMsg(Message msg);
	};

};
