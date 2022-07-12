#include <thread>
#include <mutex>
#include <deque>
#include <vector>
#include <memory>
#include <iostream>

//Enitity
class Entity
{
public:
	Entity() {
        std::cout<<"Construct Entity"<<std::endl;
    };
	virtual ~Entity(){
        std::cout<<"                Destruct Entity"<<std::endl;
    };

protected:
	int x, y;
};

//Threadsave Deque
template<typename T>
class TSDeque
{
public:
	TSDeque() = default;
    TSDeque(const TSDeque<T> &) = delete;
    TSDeque(TSDeque<T> &&new_item)
    {
        std::scoped_lock lock(mux_que);
        que = std::move(mux_que);
    }
	~TSDeque() = default;

    TSDeque &operator=(TSDeque<T> &&new_item)
    {
        std::scoped_lock lock(mux_que);
        que = std::move(mux_que);
        return *this;
    }

	void push_back(const T &item)
	{
		std::scoped_lock lock(mux_que);
		que.emplace_back(std::move(item));
	}

	T pop_front()
	{
		std::scoped_lock lock(mux_que);
		auto t = std::move(que.front());
		que.pop_front();
		return t;
	}

    bool empty()
    {
        return que.empty();
    }
protected:
	std::mutex mux_que;
	std::deque<T> que;
};

//main loop
int main()
{
    std::deque<std::vector<Entity>> updates_buffer(100, std::vector<Entity>(10));
	std::vector<TSDeque<std::shared_ptr<std::vector<Entity>>>> entity_buffer(2);
	std::thread update_thread, worker_thread;
        
    update_thread = std::thread([&updates_buffer, &entity_buffer]()
    {
        while(!updates_buffer.empty()){
            std::shared_ptr<std::vector<Entity>> temp_buf = std::make_shared<std::vector<Entity>>(std::move(updates_buffer.front()));
            entity_buffer.at(0).push_back(temp_buf);
            updates_buffer.pop_front();    
        }
    });

    worker_thread = std::thread([&entity_buffer, &updates_buffer]()
    {
        std::shared_ptr<std::vector<Entity>> current_change;
        while(!entity_buffer.at(0).empty() || !updates_buffer.empty())
        {
            current_change = entity_buffer.at(0).pop_front();
            //do some work wiht current_change
        }
    });

    if(update_thread.joinable())
        update_thread.join();
    if(worker_thread.joinable())
        worker_thread.join();
    std::cin.get();

    return 1;
}