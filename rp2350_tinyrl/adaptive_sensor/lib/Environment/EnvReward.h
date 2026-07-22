#ifndef ENV_REWARD_H
#define ENV_REWARD_H

class EnvReward {
public:
    EnvReward();
    ~EnvReward();

    float calculateReward();
    void reset();

private:
    float reward;
};

#endif // ENV_REWARD_H
