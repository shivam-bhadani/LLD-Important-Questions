// State Design Pattern

// IdleState -> AcceptingCoinState -> ProductSelectionState -> DispensingState -> IdleState

// IdleState has pressInsertCoinButton change state to AcceptingCoinState

// AcceptingCoinState has insertCoin() then pressSelectProductButton() to change state to ProductSelectionState

// ProductSelectionState has selectProduct() to chnage state to DispensingState

// DispensingState has collectProduct() to change state to IdleState

// AcceptingCoinState and ProductSelectionState has cancelRequest() that return coins and changeS state to IdleState

class Coin {
protected:
    int value;
};

class OneRupee : public Coin {
public:
    OneRupee() {
        value = 1;
    }
};

class FiveRupees : public Coin {
public:
    FiveRupees() {
        value = 5;
    }
};

class TenRupees : public Coin {
public:
    TenRupees() {
        value = 10;
    }
};

class Item {
public:
    string type;
    int price;

    int getPrice() {
        return price;
    }
};

class Coke : public Item {
public:
    Coke() {
        type = "Coke";
        price = 5;
    }
};

class Soda : public Item {
public:
    Soda() {
        type = "Soda";
        price = 10;
    }
};

class State {
public:
    virtual void pressInsertCoinButton(VendingMachine *machine) {
        cout << "Method not supported" << endl;
    }
    virtual void insertCoin(VendingMachine *machine, Coin *coin) {
        cout << "Method not supported" << endl;
    }
    virtual void pressSelectProductButton(VendingMachine *machine) {
        cout << "Method not supported" << endl;
    }
    virtual void selectProduct(VendingMachine *machine, int code) {
        cout << "Method not supported" << endl;
    }
    virtual Item* collectProduct(VendingMachine *machine, int code) {
        cout << "Method not supported" << endl;
    }
    virtual vector<Coin*> cancelRequest(VendingMachine *machine) {
        cout << "Method not supported" << endl;
        return {};
    }
    virtual int getChange(int returnChangeMoney) {
        cout << "Method not supported" << endl;
    }
};

class IdleState : public State {
public:
    void IdleState() {
        cout << "Vending Machine is in IdleState" << endl;
    }
    void IdleState(VendingMachine *machine) {
        cout << "Vending Machine is in IdleState" << endl;
        machine->resetCoinList();
    }

    void pressInsertCoinButton(VendingMachine *machine) {
        machine->setVendingMachineState(new AcceptingCoinState());
    }
};

class AcceptingCoinState : public State {
public:
    AcceptingCoinState() {
        cout << "Vending Machine is in AcceptingCoinState" << endl;
    }

    void insertCoin(VendingMachine *machine, Coin *coin) {
        cout << "Accepting Coin " << coin->value << endl;
        machine->addCoinToCoinList(coin);
    }

    void pressSelectProductButton(VendingMachine *machine) {
        machine->setVendingMachineState(new ProductSelectionState());
    }

    vector<Coin*> cancelRequest(VendingMachine *machine) {
        cout << "Cancel Request and Returning the money" << endl;
        machine->setVendingMachineState(new IdleState(machine));
        return machine->getCoinList();
    }
};

class ProductSelectionState : public State {
public:
    ProductSelectionState() {
        cout << "Vending Machine is in ProductSelectionState" << endl;
    }

    void selectProduct(VendingMachine *machine, int code) {
        // get item from inventory
        Item *item = machine->getItemFromInventory(code);

        // total amount paid by the customer
        int amountPaidByCustomer = 0;
        vector<Coin*> coinsInVendingMachine = machine->getCoinList();
        for (auto& coin : coinsInVendingMachine) {
            amountPaidByCustomer += coin->value;
        }

        // comparing price and amount paid by the customer
        if (amountPaidByCustomer < item->getPrice()) {
            cancelRequest(machine);
            cout << "Insufficient Money" << endl;
        }
        else {
            if (amountPaidByCustomer > item->getPrice()) {
                int changeReturned = getChange(amountPaidByCustomer - item->getPrice());
                cout << "Amount of extra change returned is: " << changeReturned << endl;
            }
            machine->setVendingMachineState(new DispensingState(machine, code));
        }
    }

    int getChange(int amount) {
        return amount;
    }

    vector<Coin*> cancelRequest(VendingMachine *machine) {
        cout << "Cancel Request and Returning the money" << endl;
        machine->setVendingMachineState(new IdleState(machine));
        return machine->getCoinList();
    }
};

class DispensingState : public State {
public:
    DispensingState(VendingMachine *machine, int code) {
        cout << "Vending Machine is in DispensingState" << endl;
        collectProduct(machine, code);
    }
    Item* collectProduct(VendingMachine *machine, int code) {
        Item *item = machine->getItemFromInventory(code);
        cout << "Collecting Item: " << item->type << endl;
        machine->setVendingMachineState(new IdleState(machine));
        return item;
    }
};

class VendingMachine {
private:
    vector<Coin*> coinList;
    State *vendingMachineState;
    map<int, Item*> inventory;

public:
    VendingMachine() {
        coinList.clear();
        vendingMachineState = new IdleState();

        // Hard coding inventory for simplicity
        for (int i = 0; i < 6; i++) {
            if (i < 3) {
                inventory[i] = new Coke();
            }
            else {
                inventory[i] = new Soda();
            }
        }
    }

    State* getVendingMachineState() {
        return vendingMachineState;
    }

    void setVendingMachineState(State *state) {
        vendingMachineState = state;
    }

    Item* getItemFromInventory(int code) {
        return inventory[code];
    }

    vector<Coin*> getCoinList() {
        return coinList;
    }

    void addCoinToCoinList(Coin *coin) {
        coinList.push_back(coin);
    }

    void resetCoinList() {
        coinList.clear();
    }
};
