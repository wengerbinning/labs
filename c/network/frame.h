
typedef union frame_flags {
	unsigned int value;
	struct {
		unsigned int root: 1;
	};
} frame_flags_t;