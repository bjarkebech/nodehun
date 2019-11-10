#include <napi.h>

#include <chrono>
#include <thread>
#include <hunspell.hxx>
#include "Worker.cc"

class SpellWorker : public Worker {
    public:
        SpellWorker(
            HunspellContext* context,
            Napi::Promise::Deferred d,
            std::string word)
        : Worker(context, d), word(word) {}

    void Execute() {
        // Worker thread; don't use N-API here
        context->lock();
        correct = context->instance->spell(word.c_str());
        context->unlock();
    }

    void Resolve(Napi::Promise::Deferred const &deferred) {
        Napi::Env env = deferred.Env();

        deferred.Resolve(Napi::Boolean::New(env, correct));
    }

    private:
        bool correct = false;
        std::string word;
};