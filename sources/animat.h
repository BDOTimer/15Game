#ifndef animat_h
#define animat_h
/// СТАТУС: тест
///----------------------------------------------------------------------------|
/// "animat.h"
///
///     - Инициализация котрола анимации:
///             переменная ... val
///             расстояние ... D
///             скорость   ... speed
///     - Событие старта ...   start  ()
///     - Процесс ...          process()
///     - Финиш ...            finish ()
///
///     И ещё три функциональные переменные:
///         callb_is     - условие окончания анимации.
///         callb_calc   - форма анимации.
///         callb_finish - то, что нужно сделать в конце.
///
///     Связанная группа контолов:
///         варианты:
///         - только один может анимироваться в текущий момент.
///         - возможна множественная анимация.
///----------------------------------------------------------------------------:

typedef std::function<void()>  Delegate_t;
typedef std::function<bool()> BDelegate_t;

const char* err_lamda_init = "WARNING:Lamda not init\n";

struct IAnimat
{
    virtual ~IAnimat(){}

    virtual bool init(float&, const float, const float) = 0;
    virtual void start  () = 0;
    virtual bool process() = 0;
    virtual void finish () = 0;

    bool  done = false;
    float speed;

    float*  val;
    ///--------------------------|
    /// Прирост от 0 до D.       |
    ///--------------------------:
    float  D;
    float  DD;

   BDelegate_t callb_is     = [this](){l(err_lamda_init)return false;};
    Delegate_t callb_calc   = [this](){l(err_lamda_init)};
    Delegate_t callb_finish = [this](){l(err_lamda_init)};
};

///----------------------------------------------------------------------------|
/// Animat
///----------------------------------------------------------------------------:
struct  Animat : public IAnimat
{       Animat()
        {
        }
       ~Animat(){}

    bool init(float& v, const float d, const float sp)
    {   ///--------------------------|
        /// Запрет другой анимации.  |
        ///--------------------------:
        if(done) return false;

        val   = &v;
        D     =  d;
        speed = sp;
        return true;
    }

    void start  ()
    {   done = true;
        DD   = *val + D;
    }

    bool process()
    {   if(done)
        {   callb_calc();

            if(callb_is())
            {   done = false;
                finish ();
            }

            return done;
        }

        return done;
    }

    void finish ()
    {   callb_finish();
    }
};

///--------------------------|
/// Тест.                    |
///--------------------------:
struct  Test_Animat
{       Test_Animat()
        {
            pa = std::make_unique<Animat>();
            pa->init(x, 2.f, 0.1f);
            pa->callb_finish = [this]()        {make(17.f);};
            pa->callb_calc   = [p = pa.get()](){       *(p->val) += p->speed;};
            pa->callb_is     = [p = pa.get()](){return *(p->val) >  p->DD   ;};
        }

        void draw (){if(pa->process()) l(x)}
        void start(){   pa->start  ();     }

    void make(float a)
    {   x = a;
    }

    float x;
    std::unique_ptr<Animat> pa;
};

inline void testclass_Animat()
{
    Test_Animat test;

    while(true)
    {
        {
            if(myl::is_press_key('a')) test.start();
            test.draw();
        }
    }
}

#endif // animat_h
