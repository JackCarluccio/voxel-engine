#pragma once

#include <vector>
#include <functional>

namespace Engine {
	
	template<typename... Args>
	class Signal {
	public:

		// Class representing a connection to a signal slot
		class Connection {
		public:
			Connection(size_t id, Signal* signal) : id(id), signal(signal) {};

			void disconnect() {
				if (signal) {
					signal->disconnect(id);
				}
			};

		private:
			size_t id;
			Signal* signal;
		};

		Connection connect(std::function<void(Args...)> callback) {
			size_t id = nextId++;
			slots.emplace_back(id, std::move(callback));
			return Connection(id, this);
		};

		void emit(Args... args) {
			for (auto& slot : slots) {
				slot.callback(args...);
			}
		};

	private:
		struct Slot {
			size_t id;
			std::function<void(Args...)> callback;
		};

		// Disconnect a slot by its ID
		void disconnect(size_t id) {
			for (int i = 0; i < slots.size(); i++) {
				if (slots[i].id == id) {
					slots.erase(slots.begin() + i);
					return;
				}
			}
		};

		std::vector<Slot> slots;
		size_t nextId = 0;
	};

}
