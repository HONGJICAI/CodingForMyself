#include <stdint.h>
#include <string>

struct HungarianRings {
	const int N = 15, GAP = 5;
	enum COLOR {
		BLUE = 0,
		YELLOW,
		PINK,
		GREEN
	};
	enum ROTATE {
		L = 0,
		AL,
		R,
		AR,
		NONE
	};
	uint32_t left = 0, right = 0;
	HungarianRings() {

	}
	HungarianRings(uint64_t val) {
		this->left = (val >> 32) & 0xffffffff;
		this->right = val & 0xffffffff;
	}
	HungarianRings(char left[], char right[]) {
		// left[i] = 0 -- 3, occupy 2 bit
		for (int i = N - 1; i >= 0; --i) {
			this->left = (this->left << 2) | left[i];
			this->right = (this->right << 2) | right[i];
		}
	}
	HungarianRings clone(ROTATE dir) {
		HungarianRings cc(*this);
		switch (dir) {
		case ROTATE::R:cc.rotate_right(); break;
		case ROTATE::AR:cc.antirotate_right(); break;
		case ROTATE::L:cc.rotate_left(); break;
		case ROTATE::AL:cc.antirotate_left(); break;
		}
		return cc;
	}
	bool operator==(const HungarianRings &a) {
		return this->left == a.left&&this->right == a.right;
	}
	uint64_t compress() {
		return (static_cast<uint64_t>(left) << 32) | right;
	}
	static HungarianRings decompress(uint64_t val) {
		return HungarianRings(val);
	}
	COLOR getColor(const std::string &ring, int offset) const{
		if (ring == "left") {
			return COLOR((left >> (2 * offset)) & 3);
		}
		else if (ring == "right") {
			return COLOR((right >> (2 * offset)) & 3);
		}
		else throw "param ring should be 'left' or 'right'";
	}
	void setColor(const std::string &ring, int offset, COLOR col) {
		int mask = (0xffffffff ^ (3 << (offset * 2)));
		if (ring == "left") {
			left = (left & mask) | (col << (offset * 2));
		}
		else if (ring == "right") {
			right = (right & mask) | (col << (offset * 2));
		}
		else throw "param ring should be 'left' or 'right'";
	}
	bool operator()(const HungarianRings &a, const HungarianRings&b) const {
		return a.left == b.left && a.right == b.right;
	}
	HungarianRings &operator=(const HungarianRings &a) {
		this->left = a.left;
		this->right = a.right;
		return *this;
	}
	bool valid() {
		int color[4] = {};
		for (int i = 0; i < N; ++i) {
			color[getColor("left", i)]++;
			color[getColor("right", i)]++;
		}
		color[getColor("left", N - 1)]--;
		color[getColor("left", N - GAP)]--;
		return color[0] == color[1] && color[2] == color[3] && color[1] == color[2];
	}
	void rotate_left() {
		// get color of the first bead
		COLOR col = getColor("left", 0);
		// rotate the left rings clockwisely
		left >>= 2;
		setColor("left", N - 1, col);

		// modify the intersection beads
		setColor("right", N - 1, getColor("left", N - GAP));
		setColor("right", N - GAP, getColor("left", N - 1));
	}
	void rotate_right() {
		// get color of the first bead
		COLOR col = getColor("right", 0);
		// rotate the right rings clockwisely
		right >>= 2;
		setColor("right", N - 1, col);

		// modify the intersection beads
		setColor("left", N - 1, getColor("right", N - GAP));
		setColor("left", N - GAP, getColor("right", N - 1));
	}
	void antirotate_left() {
		// get color of the first bead
		COLOR col = getColor("left", N - 1);
		// rotate the left rings anticlockwisely
		left = (left << 4) >> 2;
		setColor("left", 0, col);

		// modify the intersection beads
		setColor("right", N - 1, getColor("left", N - GAP));
		setColor("right", N - GAP, getColor("left", N - 1));
	}
	void antirotate_right() {
		// get color of the first bead
		COLOR col = getColor("right", N - 1);
		// rotate the right rings anticlockwisely
		right = (right << 4) >> 2;
		setColor("right", 0, col);

		// modify the intersection beads
		setColor("left", N - 1, getColor("right", N - GAP));
		setColor("left", N - GAP, getColor("right", N - 1));
	}
	bool finish() {
		int i, color;
		for (i = 0 + 1, color = getColor("left", 0); i < 7; ++i)
			if (getColor("left", i) != color)
				return false;
		for (i = 7 + 1, color = getColor("left", 7); i < 14; ++i)
			if (getColor("left", i) != color)
				return false;
		for (i = 0 + 1, color = getColor("right", 0); i < 7; ++i)
			if (getColor("right", i) != color)
				return false;
		for (i = 7 + 1, color = getColor("right", 7); i < 14; ++i)
			if (getColor("right", i) != color)
				return false;
		return true;
	}
};