#include "base/at_exit.h"

#include "base/log/logging.h"

namespace base
{

static AtExitManager* g_top_manager = nullptr;

AtExitManager::AtExitManager() : next_manager_(g_top_manager) {
	g_top_manager = this;
}

AtExitManager::~AtExitManager() {
	if(!g_top_manager) {
		NOTREACHED() << "AtExitManager not be initialized";
		return;
	}

	DCHECK_EQ(this, g_top_manager);
	RunCallbacksNow();

	g_top_manager = next_manager_;
}

void AtExitManager::RegisterCallback(AtExitCallback func) {
	DCHECK(func);
	
	if(!g_top_manager) {
		NOTREACHED() << "AtExitManager not be initialized";
		return;
	}

	std::lock_guard<std::mutex> lock(g_top_manager->mutex_);
	g_top_manager->stack_.push(func);
}

void AtExitManager::RunCallbacksNow() {
	if(!g_top_manager) {
		NOTREACHED() << "AtExitManager not be initialized";
		return;
	}

	std::lock_guard<std::mutex> lock(g_top_manager->mutex_);
	while (!g_top_manager->stack_.empty())
	{
		auto func = g_top_manager->stack_.top();
		if(func) {
			func();
		}
		g_top_manager->stack_.pop();
	}
	
}

} // namespace base
