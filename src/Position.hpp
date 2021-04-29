#ifndef Position_hpp_HEADERGUARD
#define Position_hpp_HEADERGUARD


struct Position{
	Position():x(0),y(0),z(0){};
	Position(int x, int y, int z):x(x),y(y),z(z){};

	int x;
	int y;
	int z;

	bool operator<(const Position &rhs) const{
		const Position &lhs = *this;
		if(lhs.z != rhs.z){
			return lhs.z < rhs.z;
		} else if (lhs.y != rhs.y){
			return lhs.y < rhs.y;
		} else {
			return lhs.x < rhs.x;
		}
	};

	bool operator==(const Position &rhs) const{
		const Position &lhs = *this;

		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}
};


#endif //Position_hpp_HEADERGUARD
