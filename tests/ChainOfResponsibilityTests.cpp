#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "include/Request.h"
#include "include/StaffMembers.h"
#include "include/SalesAssistant.h"
#include "include/FloorManager.h"
#include "include/NurseryOwner.h"
#include "include/Customer.h"
#include "include/DerivedCustomers.h"
#include "include/NurseryMediator.h"

// ============ Request Tests ============

class RequestTest : public ::testing::Test {
protected:
    RegularCustomer* customer;
    Request* request;
    
    void SetUp() override {
        customer = new RegularCustomer();
        customer->setName("Test Customer");
        customer->setId("CUST-001");
    }
    
    void TearDown() override {
        delete customer;
        if (request != nullptr) {
            delete request;
        }
    }
};

TEST_F(RequestTest, RequestCreation) {
    request = new Request("I need a Rose plant", customer);
    
    EXPECT_NE(request, nullptr);
    EXPECT_EQ(request->getMessage(), "I need a Rose plant");
    EXPECT_FALSE(request->isHandled());
}

TEST_F(RequestTest, DefaultConstructor) {
    request = new Request();
    
    EXPECT_NE(request, nullptr);
    EXPECT_TRUE(request->getMessage().empty());
}

TEST_F(RequestTest, ParseLowLevelRequest) {
    request = new Request("I want a rose plant", customer);
    
    EXPECT_EQ(request->getLevel(), RequestLevel::LOW);
}

TEST_F(RequestTest, ParseMediumLevelRequest) {
    request = new Request("I need a bulk order for wedding", customer);
    
    EXPECT_EQ(request->getLevel(), RequestLevel::MEDIUM);
}

TEST_F(RequestTest, ParseHighLevelRequest) {
    request = new Request("I want a refund immediately", customer);
    
    EXPECT_EQ(request->getLevel(), RequestLevel::HIGH);
}

TEST_F(RequestTest, ParseComplaintRequest) {
    request = new Request("I have a complaint about service", customer);
    
    EXPECT_EQ(request->getLevel(), RequestLevel::HIGH);
}

TEST_F(RequestTest, ParseUrgentRequest) {
    request = new Request("This is urgent please help", customer);
    
    EXPECT_EQ(request->getLevel(), RequestLevel::HIGH);
}

TEST_F(RequestTest, ParseManagerRequest) {
    request = new Request("I need to speak to a manager", customer);
    
    EXPECT_EQ(request->getLevel(), RequestLevel::HIGH);
}

TEST_F(RequestTest, ParseBulkOrderRequest) {
    request = new Request("I need 50 plants for corporate event", customer);
    
    EXPECT_EQ(request->getLevel(), RequestLevel::MEDIUM);
}

TEST_F(RequestTest, ParseSpecialArrangementRequest) {
    request = new Request("Can you make a special arrangement", customer);
    
    EXPECT_EQ(request->getLevel(), RequestLevel::MEDIUM);
}

TEST_F(RequestTest, MarkHandled) {
    request = new Request("Simple request", customer);
    
    EXPECT_FALSE(request->isHandled());
    
    request->markHandled();
    
    EXPECT_TRUE(request->isHandled());
}

TEST_F(RequestTest, SetLevel) {
    request = new Request("Simple request", customer);
    
    request->setLevel(RequestLevel::HIGH);
    
    EXPECT_EQ(request->getLevel(), RequestLevel::HIGH);
}

TEST_F(RequestTest, ExtractPlantNameRose) {
    request = new Request("I want a rose plant", customer);
    
    std::string plantName = request->extractPlantName();
    
    EXPECT_EQ(plantName, "Rose");
}

TEST_F(RequestTest, ExtractPlantNameCactus) {
    request = new Request("Do you have any cactus plants", customer);
    
    std::string plantName = request->extractPlantName();
    
    EXPECT_EQ(plantName, "Cactus");
}

TEST_F(RequestTest, ExtractPlantNameDaisy) {
    request = new Request("Looking for daisy flowers", customer);
    
    std::string plantName = request->extractPlantName();
    
    EXPECT_EQ(plantName, "Daisy");
}

TEST_F(RequestTest, ExtractPlantNameNoMatch) {
    request = new Request("Hello how are you", customer);
    
    std::string plantName = request->extractPlantName();
    
    EXPECT_TRUE(plantName.empty());
}

TEST_F(RequestTest, GetCustomer) {
    request = new Request("Test request", customer);
    
    Customer* retrievedCustomer = request->getCustomer();
    
    EXPECT_EQ(retrievedCustomer, customer);
}

TEST_F(RequestTest, ToStringFormat) {
    request = new Request("Test message", customer);
    
    std::string str = request->toString();
    
    EXPECT_NE(str.find("Test message"), std::string::npos);
    EXPECT_NE(str.find("Test Customer"), std::string::npos);
}

TEST_F(RequestTest, CaseInsensitiveKeywords) {
    request = new Request("I NEED A ROSE PLANT", customer);
    
    EXPECT_EQ(request->getLevel(), RequestLevel::LOW);
    EXPECT_EQ(request->extractPlantName(), "Rose");
}

TEST_F(RequestTest, KeywordWithPunctuation) {
    request = new Request("I want a rose, please!", customer);
    
    std::string plantName = request->extractPlantName();
    
    EXPECT_EQ(plantName, "Rose");
}

TEST_F(RequestTest, MultipleKeywords) {
    request = new Request("I want a refund for this rose plant complaint", customer);
    
    // Should prioritize HIGH level keywords
    EXPECT_EQ(request->getLevel(), RequestLevel::HIGH);
}

// ============ Chain of Responsibility Tests ============

class ChainOfResponsibilityTest : public ::testing::Test {
protected:
    NurseryMediator* mediator;
    SalesAssistant* assistant;
    FloorManager* manager;
    NurseryOwner* owner;
    RegularCustomer* customer;
    
    void SetUp() override {
        mediator = new NurseryMediator();
        
        assistant = new SalesAssistant(mediator, "Alice", "SA-001");
        manager = new FloorManager(mediator, "Bob", "FM-001");
        owner = new NurseryOwner(mediator, "Carol", "NO-001");
        
        // Set up chain
        assistant->setNext(manager);
        manager->setNext(owner);
        
        customer = new RegularCustomer();
        customer->setMediator(mediator);
        customer->setName("Test Customer");
        customer->setId("CUST-001");
        
        mediator->registerColleague(assistant);
        mediator->registerColleague(manager);
        mediator->registerColleague(owner);
        mediator->registerColleague(customer);
    }
    
    void TearDown() override {
        delete customer;
        delete owner;
        delete manager;
        delete assistant;
        delete mediator;
    }
};

TEST_F(ChainOfResponsibilityTest, ChainSetup) {
    EXPECT_NE(assistant, nullptr);
    EXPECT_NE(manager, nullptr);
    EXPECT_NE(owner, nullptr);
}

TEST_F(ChainOfResponsibilityTest, LowLevelRequestHandledByAssistant) {
    Request* request = new Request("I want a rose plant", customer);
    
    assistant->handleRequest(request);
    
    EXPECT_TRUE(request->isHandled());
}

TEST_F(ChainOfResponsibilityTest, MediumLevelRequestEscalated) {
    Request* request = new Request("I need a bulk order for wedding", customer);
    
    assistant->handleRequest(request);
    
    EXPECT_TRUE(request->isHandled());
}

TEST_F(ChainOfResponsibilityTest, HighLevelRequestEscalated) {
    Request* request = new Request("I want a refund immediately", customer);
    
    assistant->handleRequest(request);
    
    EXPECT_TRUE(request->isHandled());
}

TEST_F(ChainOfResponsibilityTest, NullRequestHandling) {
    EXPECT_NO_THROW(assistant->handleRequest(nullptr));
}

// ============ SalesAssistant Tests ============

class SalesAssistantTest : public ::testing::Test {
protected:
    NurseryMediator* mediator;
    SalesAssistant* assistant;
    
    void SetUp() override {
        mediator = new NurseryMediator();
        assistant = new SalesAssistant(mediator, "Alice", "SA-001");
    }
    
    void TearDown() override {
        delete assistant;
        delete mediator;
    }
};

TEST_F(SalesAssistantTest, SalesAssistantCreation) {
    EXPECT_NE(assistant, nullptr);
    EXPECT_EQ(assistant->getName(), "Alice");
    EXPECT_EQ(assistant->getId(), "SA-001");
}

TEST_F(SalesAssistantTest, HandleLowLevelRequest) {
    RegularCustomer* customer = new RegularCustomer();
    customer->setMediator(mediator);
    
    Request* request = new Request("I want a rose", customer);
    
    assistant->handleRequest(request);
    
    EXPECT_TRUE(request->isHandled());
    
    delete customer;
}

TEST_F(SalesAssistantTest, EscalateMediumRequest) {
    FloorManager* manager = new FloorManager(mediator, "Bob", "FM-001");
    assistant->setNext(manager);
    
    RegularCustomer* customer = new RegularCustomer();
    customer->setMediator(mediator);
    
    Request* request = new Request("I need bulk order", customer);
    
    assistant->handleRequest(request);
    
    EXPECT_TRUE(request->isHandled());
    
    delete manager;
    delete customer;
}

TEST_F(SalesAssistantTest, GetName) {
    EXPECT_EQ(assistant->getName(), "Alice");
}

TEST_F(SalesAssistantTest, GetId) {
    EXPECT_EQ(assistant->getId(), "SA-001");
}

TEST_F(SalesAssistantTest, HandleRequestWithoutChain) {
    RegularCustomer* customer = new RegularCustomer();
    customer->setMediator(mediator);
    
    Request* highRequest = new Request("I want a refund", customer);
    
    // Should not crash even without next handler
    EXPECT_NO_THROW(assistant->handleRequest(highRequest));
    
    delete customer;
}

TEST_F(SalesAssistantTest, RunCareScheduler) {
    EXPECT_NO_THROW(assistant->runCareScheduler());
}

// ============ FloorManager Tests ============

class FloorManagerTest : public ::testing::Test {
protected:
    NurseryMediator* mediator;
    FloorManager* manager;
    
    void SetUp() override {
        mediator = new NurseryMediator();
        manager = new FloorManager(mediator, "Bob", "FM-001");
    }
    
    void TearDown() override {
        delete manager;
        delete mediator;
    }
};

TEST_F(FloorManagerTest, FloorManagerCreation) {
    EXPECT_NE(manager, nullptr);
    EXPECT_EQ(manager->getName(), "Bob");
    EXPECT_EQ(manager->getId(), "FM-001");
}

TEST_F(FloorManagerTest, HandleMediumLevelRequest) {
    RegularCustomer* customer = new RegularCustomer();
    
    Request* request = new Request("I need bulk order for wedding", customer);
    
    manager->handleRequest(request);
    
    EXPECT_TRUE(request->isHandled());
    
    delete customer;
}

TEST_F(FloorManagerTest, HandleLowLevelRequest) {
    RegularCustomer* customer = new RegularCustomer();
    
    Request* request = new Request("I want a rose", customer);
    
    manager->handleRequest(request);
    
    EXPECT_TRUE(request->isHandled());
    
    delete customer;
}

TEST_F(FloorManagerTest, EscalateHighRequest) {
    NurseryOwner* owner = new NurseryOwner(mediator, "Carol", "NO-001");
    manager->setNext(owner);
    
    RegularCustomer* customer = new RegularCustomer();
    
    Request* request = new Request("I want a refund", customer);
    
    manager->handleRequest(request);
    
    EXPECT_TRUE(request->isHandled());
    
    delete owner;
    delete customer;
}

TEST_F(FloorManagerTest, GetName) {
    EXPECT_EQ(manager->getName(), "Bob");
}

TEST_F(FloorManagerTest, GetId) {
    EXPECT_EQ(manager->getId(), "FM-001");
}

TEST_F(FloorManagerTest, HandleNullRequest) {
    EXPECT_NO_THROW(manager->handleRequest(nullptr));
}

// ============ NurseryOwner Tests ============

class NurseryOwnerTest : public ::testing::Test {
protected:
    NurseryMediator* mediator;
    NurseryOwner* owner;
    
    void SetUp() override {
        mediator = new NurseryMediator();
        owner = new NurseryOwner(mediator, "Carol", "NO-001");
    }
    
    void TearDown() override {
        delete owner;
        delete mediator;
    }
};

TEST_F(NurseryOwnerTest, NurseryOwnerCreation) {
    EXPECT_NE(owner, nullptr);
    EXPECT_EQ(owner->getName(), "Carol");
    EXPECT_EQ(owner->getId(), "NO-001");
}

TEST_F(NurseryOwnerTest, HandleHighLevelRequest) {
    RegularCustomer* customer = new RegularCustomer();
    
    Request* request = new Request("I want a refund", customer);
    
    owner->handleRequest(request);
    
    EXPECT_TRUE(request->isHandled());
    
    delete customer;
}

TEST_F(NurseryOwnerTest, HandleComplaintRequest) {
    RegularCustomer* customer = new RegularCustomer();
    
    Request* request = new Request("I have a complaint", customer);
    
    owner->handleRequest(request);
    
    EXPECT_TRUE(request->isHandled());
    
    delete customer;
}

TEST_F(NurseryOwnerTest, HandleUrgentRequest) {
    RegularCustomer* customer = new RegularCustomer();
    
    Request* request = new Request("This is urgent", customer);
    
    owner->handleRequest(request);
    
    EXPECT_TRUE(request->isHandled());
    
    delete customer;
}

TEST_F(NurseryOwnerTest, HandleAnyRequest) {
    RegularCustomer* customer = new RegularCustomer();
    
    // Owner handles any level since they're at the top
    Request* lowRequest = new Request("I want a plant", customer);
    
    owner->handleRequest(lowRequest);
    
    EXPECT_TRUE(lowRequest->isHandled());
    
    delete customer;
}

TEST_F(NurseryOwnerTest, GetName) {
    EXPECT_EQ(owner->getName(), "Carol");
}

TEST_F(NurseryOwnerTest, GetId) {
    EXPECT_EQ(owner->getId(), "NO-001");
}

TEST_F(NurseryOwnerTest, HandleNullRequest) {
    EXPECT_NO_THROW(owner->handleRequest(nullptr));
}

// ============ StaffMembers Base Class Tests ============

class StaffMembersTest : public ::testing::Test {
protected:
    NurseryMediator* mediator;
    SalesAssistant* staff1;
    SalesAssistant* staff2;
    
    void SetUp() override {
        mediator = new NurseryMediator();
        staff1 = new SalesAssistant(mediator, "Staff1", "S1");
        staff2 = new SalesAssistant(mediator, "Staff2", "S2");
    }
    
    void TearDown() override {
        delete staff2;
        delete staff1;
        delete mediator;
    }
};

TEST_F(StaffMembersTest, SetNext) {
    EXPECT_NO_THROW(staff1->setNext(staff2));
}

TEST_F(StaffMembersTest, SetNullNext) {
    EXPECT_NO_THROW(staff1->setNext(nullptr));
}

TEST_F(StaffMembersTest, ChainLinking) {
    staff1->setNext(staff2);
    
    RegularCustomer* customer = new RegularCustomer();
    Request* request = new Request("escalate this", customer);
    request->setLevel(RequestLevel::MEDIUM);
    
    // Should escalate from staff1 to staff2
    EXPECT_NO_THROW(staff1->handleRequest(request));
    
    delete customer;
}

// ============ Integration Tests ============

TEST_F(ChainOfResponsibilityTest, FullChainIntegration) {
    Request* lowRequest = new Request("I want a rose", customer);
    Request* mediumRequest = new Request("I need bulk order", customer);
    Request* highRequest = new Request("I want a refund", customer);
    
    assistant->handleRequest(lowRequest);
    assistant->handleRequest(mediumRequest);
    assistant->handleRequest(highRequest);
    
    EXPECT_TRUE(lowRequest->isHandled());
    EXPECT_TRUE(mediumRequest->isHandled());
    EXPECT_TRUE(highRequest->isHandled());
}

TEST_F(ChainOfResponsibilityTest, CustomerCreatesAndSubmitsRequest) {
    Request* request = customer->createRequest("I need help with a rose plant");
    
    EXPECT_NE(request, nullptr);
    EXPECT_EQ(customer->getCurrentRequest(), request);
    
    customer->submitRequestToStaff(assistant);
    
    EXPECT_TRUE(request->isHandled());
}

TEST_F(ChainOfResponsibilityTest, CustomerReceivesResponse) {
    Request* request = customer->createRequest("I want a plant");
    
    testing::internal::CaptureStdout();
    customer->receiveResponse("Here is your plant!");
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("Here is your plant!"), std::string::npos);
    EXPECT_TRUE(request->isHandled());
}

TEST_F(ChainOfResponsibilityTest, MultipleRequestsSequentially) {
    Request* request1 = new Request("I want a rose", customer);
    Request* request2 = new Request("I need a bulk order", customer);
    
    assistant->handleRequest(request1);
    assistant->handleRequest(request2);
    
    EXPECT_TRUE(request1->isHandled());
    EXPECT_TRUE(request2->isHandled());
}

TEST_F(ChainOfResponsibilityTest, RequestWithoutCustomer) {
    Request* request = new Request();
    request->parseRequest("I want a plant");
    
    EXPECT_NO_THROW(assistant->handleRequest(request));
    
    delete request;
}