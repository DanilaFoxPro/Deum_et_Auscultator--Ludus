#ifndef __ROLES_H_
#define __ROLES_H_

namespace role {
	
	namespace chat {
		
		enum {
			
			none = -1,
			mute,
			
		};
		
	}
	
	namespace interception {
		
		enum {
			
			none = role::chat::none,
			info,
			intr,
			rcvr,
			
		};
		
	}
	
}

#endif
