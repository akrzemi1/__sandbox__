struct module
{
    module&& operator()() && { return std::move(*this); }
    module&& operator[]() && { return std::move(*this); }
    void operator=(void(*)()) {}
};
