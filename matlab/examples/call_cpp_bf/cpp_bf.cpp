#include "mex.hpp"
#include "mexAdapter.hpp"

#include "../../../cpp/BFMachineLib/BFMachineLib.hpp"

#include <string>
#include <vector>

class MexFunction : public matlab::mex::Function
{
    public:
        void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs)
        {
            std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
            matlab::data::ArrayFactory factory;
            if (inputs.size() != 2)
            {
                    matlabPtr->feval(
                        u"error", 0,
                        std::vector<matlab::data::Array>(
                            {factory.createScalar("Two inputs required.")}));
            }
            if (outputs.size() != 1)
            {
                    matlabPtr->feval(
                        u"error", 0,
                        std::vector<matlab::data::Array>(
                            {factory.createScalar("One output required.")}));
            }
            if (inputs[0].getType() != matlab::data::ArrayType::CHAR)
            {
                matlabPtr->feval(
                    u"error", 0,
                    std::vector<matlab::data::Array>(
                        {factory.createScalar("Wrong bf_code type.")}));
            }
            switch(inputs[1].getType())
            {
                case matlab::data::ArrayType::UINT32:
                    this->perform_computation<std::uint32_t>(outputs, inputs);
                    break;
                case matlab::data::ArrayType::UINT64:
                    this->perform_computation<std::uint64_t>(outputs, inputs);
                    break;
                case matlab::data::ArrayType::CHAR:
                    this->perform_computation<char16_t>(outputs, inputs);
                    break;
                default:
                    matlabPtr->feval(
                        u"error", 0,
                        std::vector<matlab::data::Array>(
                            {factory.createScalar("Unknown input data type.")}));
            }
        }

        template<typename ValueType>
        void perform_computation(
                matlab::mex::ArgumentList outputs,
                matlab::mex::ArgumentList inputs) const
        {
            const matlab::data::CharArray raw_bf_code = std::move(inputs[0]);
            const std::string bf_code = raw_bf_code.toAscii();
            using value_type = ValueType;
            const matlab::data::TypedArray<value_type> tmp_input = std::move(inputs[1]);
            const std::vector<value_type> input_vec(tmp_input.begin(), tmp_input.end());


            using memory_type = bfm::memory_types::VectorMemory<std::vector<value_type> >;
            using i_stream_type = bfm::streams::InputStream<decltype(input_vec) >;
            using o_stream_type = bfm::streams::OutputVectorStream<std::vector<value_type> >;
            using bfm_type = bfm::BFMachine<memory_type, i_stream_type, o_stream_type>;

            i_stream_type input_stream = i_stream_type(input_vec);
            o_stream_type output_stream = o_stream_type();
            bfm_type(input_stream, output_stream).execute_optimized(bf_code);

            matlab::data::ArrayFactory factory;
            const auto tmp_res = output_stream.get_data();
            const matlab::data::TypedArray<value_type> res_data =
                factory.createArray({1, tmp_res.size()}, tmp_res.begin(), tmp_res.end());
            outputs[0] = res_data;
        }
};
