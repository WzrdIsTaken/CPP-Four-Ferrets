// Created by Ben on 24/01/2022

#include <Engine/Logger.hpp>

#include "../../../include/ASGEGameLib/handlers/ThreadHandler.hpp"

bcw::ThreadHandler::ThreadHandler(const unsigned int num_of_threads)
{
  // By default, num_of_threads will be set to std::thread::hardware_concurrency() in DriverSettings

  if (num_of_threads > std::thread::hardware_concurrency())
  {
    Logging::WARN(
      "num_of_threads shouldn't be greater than the hardware concurrency limit! "
      "Requested threads: " +
      std::to_string(num_of_threads) +
      " | Hardware Limit: " + std::to_string(std::thread::hardware_concurrency()));
  }

  for (unsigned int i = 0; i < num_of_threads; i++)
  {
    thread_pool.emplace_back(&ThreadHandler::handleJob, this); // https://bit.ly/3KEqa73
  }
}

bcw::ThreadHandler::~ThreadHandler()
{
  if (!terminate_pool)
  {
    destroyThreads();
  }
}

void bcw::ThreadHandler::scheduleJob(
  const std::function<void()>& job, const bool create_thread_if_available)
{
  {
    std::unique_lock<std::mutex> lock(queue_mutex);

    unsigned int requested_number_of_jobs = getNumberOfJobs() + 1;
    if (
      create_thread_if_available && requested_number_of_jobs < getSize() &&
      requested_number_of_jobs < std::thread::hardware_concurrency())
    {
      // this is untested but i believe
      thread_pool.emplace_back(&ThreadHandler::handleJob, this);
    }

    jobs_queue.push(job);
  }

  data_condition.notify_one();
}

void bcw::ThreadHandler::destroyThreads()
{
  { // Scopes used because the mutex will automatically unlock once it's out of scope
    std::unique_lock<std::mutex> lock(queue_mutex);
    terminate_pool = true;
  }

  data_condition.notify_all();

  for (std::thread& thread : thread_pool)
  {
    thread.join();
  }

  thread_pool.clear();
}

void bcw::ThreadHandler::handleJob()
{
  std::function<void()> job;

  while (true)
  {
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      data_condition.wait(lock, [this]() { return !jobs_queue.empty() || terminate_pool; });

      if (terminate_pool && jobs_queue.empty())
      {
        // Required or bad segmentation fault on ThreadHandler destruction
        // because we will be trying to pop an empty queue
        return;
      }

      job = jobs_queue.front();
      jobs_queue.pop();
    }

    job();
  }
}

unsigned int bcw::ThreadHandler::getSize() const
{
  return static_cast<unsigned int>(thread_pool.size());
}

unsigned int bcw::ThreadHandler::getNumberOfJobs() const
{
  return static_cast<unsigned int>(jobs_queue.size());
}
