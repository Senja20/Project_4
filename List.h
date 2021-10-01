#ifndef PROJECT_4_LIST_H
#define PROJECT_4_LIST_H

template <typename T>

class List
{
private:
    T* value;
    List* next;
    List* prev;
    List* last;
    List* first;
    int length = 0;

public:
    List()
    {
    }

    List* GetNext(List* current)
    {
        return current->next;
    }

    int GetLength()
    {
        return this->length;
    }

    void Push(T* value)
    {
        if (this->length == 0)
        {
            this->Init(this);
            this->value = value;
            this->prev = nullptr;
            this->last = this;
            this->length = 1;

            return;
        }

        List* newNode = new List();

        newNode->Push(value);

        newNode->Init(this);

        newNode->prev = this->last;

        this->last->next = newNode;

        this->last = newNode;

        this->length += 1;
    }

    T* Pop()
    {
        if (this->length == 0)
        {
            return nullptr;
        }

        List* res = this->last;

        T* value = res->value;

        res->Init(this);

        this->last = res->prev;

        this->length -= 1;

        if (res != this)
        {
            delete res;
        }
        else
        {
            this->value = nullptr;
            this->first = nullptr;
            this->last = nullptr;
            this->next = nullptr;
        }

        return value;
    }

    void Empty()
    {
        while (this->GetLength() != 0)
        {
            T* value = this->Pop();
        }
    }

    T* GetLast()
    {
        return this->last->value;
    }

    T* AddFirst()
    {
        T* newPoint = this->first->value;

        this->Push(newPoint);
    }

    List<T>* Clone(int from)
    {
        List<T>* newList = new List<T>();

        List<T>* next = this->first;

        int i = 0;

        while (next != nullptr)
        {
            if (i < from)
            {
                i += 1;
            }
            else
            {
                newList->Push(next->value);
            }

            next = this->GetNext(next);
        }

        return newList;
    }
private:
    void Init(List* first)
    {
        this->next = nullptr;
        this->last = nullptr;
        this->first = first;
    }
};

#endif //PROJECT_4_LIST_H
